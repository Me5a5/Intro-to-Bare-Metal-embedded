# Intro-to-Bare-Metal-embedded — `blink-led` & `Switch-led` Branches

**Repo:** [Me5a5/Intro-to-Bare-Metal-embedded](https://github.com/Me5a5/Intro-to-Bare-Metal-embedded)
**Target MCU:** STM32F401CCU6 (Cortex-M4, 256 KB Flash / 64 KB RAM — "Black Pill"-style board)
**Style:** Pure bare-metal, direct register access — no HAL, no CMSIS device header, no `RCC_Init()` struct. Everything is a hand-written `#define` to a physical address.

The repo currently has three branches (`ADC`, `Switch-led`, `blink-led`). This doc covers the two you asked about — note the actual branch name is **`Switch-led`**, not `switches`.

---

## 1. Shared toolchain & project structure

Both branches share an identical build system and support layer (confirmed via `git diff` — zero differences in these files):

```
program/
├── APP/main.c          ← the only file that changes between branches
├── MCAL/RCC/RCC.h       ← clock-enable helper (identical both branches)
└── Lib/BIT_MATH.h       ← bit-manipulation macros (identical both branches)
scripts/
├── Makefile             ← arm-none-eabi-gcc build → .elf → .hex
├── STM32F401XX_FLASH.ld ← linker script (Flash @0x08000000/256K, RAM @0x20000000/64K)
├── startup_stm32f401xx.s← reset handler + vector table
├── system_init_stm32f4xx.c
└── build.bat
```

**Build flow:** `Reset_Handler` (startup .s) sets the stack pointer → calls `SystemInit()` → copies `.data` from Flash to RAM → zeroes `.bss` → calls `main()`. The Makefile targets Cortex-M4 with hardware FPU (`fpv4-sp-d16`, `hard` float ABI) and recursively globs every `.c` under `program/`.

One thing to flag: `build.bat` runs `make sync`, but the `Makefile` has no `sync` target defined (only `all`, `src`, `build`, `clean`). Running `build.bat` as-is will fail on that line — worth fixing or removing before handing this to students.

### `Lib/BIT_MATH.h`
Standard MCAL-style register bit macros:
```c
SET_BIT(REG, BIT)     // REG |= (1 << BIT)
CLEAR_BIT(REG, BIT)   // REG &= ~(1 << BIT)
TOGGLE_BIT(REG, BIT)  // REG ^= (1 << BIT)
GET_BIT(REG, BIT)     // (REG >> BIT) & 1
```

### `MCAL/RCC/RCC.h` — `RCC_Init()`
Rather than enabling one peripheral clock at a time, this header ORs together every implemented enable bit for `AHB1ENR`, `AHB2ENR`, `APB1ENR`, and `APB2ENR` and writes all four registers once in `RCC_Init()`. It also exposes a lower-level `RCC_EnableClock(reg, bit)` for enabling a single peripheral. No `HSEON`/`PLLON`/`SW` switching happens — the chip stays on its default 16 MHz HSI, so there's no `*RDY` flag to poll.

---

## 2. Branch: `blink-led`

**Goal:** sequentially blink 3 LEDs on **GPIOA pins 0–2**.

`main.c` manually defines the GPIOA registers it needs from the base address `0x40020000`: `MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`, `ODR`.

- **`setup()`** — for each of PA0–PA2:
  - `MODER` → `01` (general-purpose output)
  - `OTYPER` → `0` (push-pull)
  - `OSPEEDR` → `00` (low speed)
  - `PUPDR` → `01` (pull-up — see note below)
- **`loop()`** — for each pin: set ODR high → busy-wait `delay(1000)` → clear ODR low → busy-wait `delay(1000)`. LEDs blink one after another, not simultaneously.
- **`delay()`** is a `volatile`-counted busy loop, not a timer/SysTick delay — so "1000" is a cycle count, not a calibrated millisecond value. It'll change if you change optimization flags or clock speed.

This branch also ships `STM32F401CCU6 pinout.jpg` for pin reference — that image isn't present on `Switch-led`.

**Minor note:** `PUPDR` is set to pull-up on pins that are already push-pull outputs. In output mode the pull resistor config is electrically inert, but it's a bit confusing to read — `00` (no pull) is the cleaner convention for outputs.

---

## 3. Branch: `Switch-led`

**Goal:** extend the above — read 3 pushbuttons on **GPIOB pins 0–2** and mirror their live state onto the 3 LEDs on **GPIOA pins 0–2**. No blinking; it's a direct, continuous input→output mapping.

Adds `GPIOB_MODER`, `GPIOB_PUPDR`, `GPIOB_IDR` (base `0x40020400`).

- **`setup()`** — GPIOA side is identical to `blink-led` (LED outputs). GPIOB side, for each pin:
  - `MODER` → `00` (input mode)
  - `PUPDR` → `01` (pull-up enabled)
- **`loop()`** — for each pin: `GET_BIT(GPIOB_IDR, pin)` → if `1`, `SET_BIT` the matching LED; if `0`, `CLEAR_BIT` it. Pure polling, no debouncing, no interrupts.

**Worth double-checking on hardware:** with an internal pull-up enabled, an unpressed/floating button reads **1** (LED on by default), and a button wired to pull the pin to GND on press reads **0** (LED turns off on press). If your physical buttons are wired the other way (pulled to VCC on press), this logic will read inverted from what you'd expect — worth confirming against your actual wiring before assuming a bug.

---

## 4. `blink-led` vs `Switch-led` at a glance

| Aspect | `blink-led` | `Switch-led` |
|---|---|---|
| GPIO ports used | GPIOA only (output) | GPIOA (output) + GPIOB (input) |
| Behavior | Sequential blink, one LED at a time | Real-time button → LED mirroring |
| Timing | `delay(1000)` busy-wait between transitions | None — tight polling loop |
| New concept introduced | Output config: `MODER`/`OTYPER`/`OSPEEDR`/`PUPDR`/`ODR` | Input config: `MODER=00`, `PUPDR` pull-up, reading `IDR` |
| Extra assets | `STM32F401CCU6 pinout.jpg` | — |

---

## 5. Suggestions if you keep building on this

- **Power/clock hygiene:** `RCC_Init()` turns on every peripheral clock unconditionally. Fine for bring-up, but for a competition board (e.g. an ROV electronics stack where every mA matters) switch to `RCC_EnableClock(RCC_AHB1ENR, RCC_AHB1ENR_GPIOAEN)` / `GPIOBEN` only — the header already exposes this.
- **Replace the busy-wait `delay()` with a SysTick-based delay** once timing actually matters (debouncing, PWM, sensor polling) — a cycle-counted loop isn't portable across clock configs.
- **Add debouncing** to `Switch-led` before using it as a template for anything competition-facing — a bare `GET_BIT` on a mechanical switch will read spurious transitions.
- **`.gitignore` for build artifacts:** `main.o`, `main.d`, `system_init_stm32f4xx.o/.d` are currently committed. If this repo is heading toward your GitHub portfolio, stripping those out (and adding `build/`, `*.o`, `*.d` to `.gitignore`) will make it read a lot cleaner.
- **Fix `build.bat`'s `make sync`** — that target doesn't exist in the Makefile, so the script currently breaks for anyone who runs it as documented.
