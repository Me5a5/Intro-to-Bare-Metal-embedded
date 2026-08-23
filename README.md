# Intro-to-Bare-Metal-embedded — `blink-led` & `Switch-led` Branches

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

### `Lib/BIT_MATH.h`
Standard MCAL-style register bit macros:
```c
SET_BIT(REG, BIT)     // REG |= (1 << BIT)
CLEAR_BIT(REG, BIT)   // REG &= ~(1 << BIT)
TOGGLE_BIT(REG, BIT)  // REG ^= (1 << BIT)
GET_BIT(REG, BIT)     // (REG >> BIT) & 1
```

## 1. Branch: `blink-led`

**Goal:** sequentially blink 3 LEDs on **GPIOA pins 0–2**.

`main.c` manually defines the GPIOA registers it needs from the base address `0x40020000`: `MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`, `ODR`.

- **`setup()`** — for each of PA0–PA2:
  - `MODER` → `01` (general-purpose output)
  - `OTYPER` → `0` (push-pull)
  - `OSPEEDR` → `00` (low speed)
  - `PUPDR` → `01` (pull-up — see note below)
- **`loop()`** — for each pin: set ODR high → busy-wait `delay(1000)` → clear ODR low → busy-wait `delay(1000)`. LEDs blink one after another, not simultaneously.
- **`delay()`** is a `volatile`-counted busy loop, not a timer/SysTick delay — so "1000" is a cycle count, not a calibrated millisecond value. It'll change if you change optimization flags or clock speed.
---

## 2. Branch: `Switch-led`

**Goal:** extend the above — read 3 pushbuttons on **GPIOB pins 0–2** and mirror their live state onto the 3 LEDs on **GPIOA pins 0–2**. No blinking; it's a direct, continuous input→output mapping.

Adds `GPIOB_MODER`, `GPIOB_PUPDR`, `GPIOB_IDR` (base `0x40020400`).

- **`setup()`** — GPIOA side is identical to `blink-led` (LED outputs). GPIOB side, for each pin:
  - `MODER` → `00` (input mode)
  - `PUPDR` → `01` (pull-up enabled)
- **`loop()`** — for each pin: `GET_BIT(GPIOB_IDR, pin)` → if `1`, `SET_BIT` the matching LED; if `0`, `CLEAR_BIT` it. Pure polling, no debouncing, no interrupts.

**Worth double-checking on hardware:** with an internal pull-up enabled, an unpressed/floating button reads **1** (LED on by default), and a button wired to pull the pin to GND on press reads **0** (LED turns off on press). If your physical buttons are wired the other way (pulled to VCC on press), this logic will read inverted from what you'd expect — worth confirming against your actual wiring before assuming a bug.

---

## 3. `blink-led` vs `Switch-led` at a glance

| Aspect | `blink-led` | `Switch-led` |
|---|---|---|
| GPIO ports used | GPIOA only (output) | GPIOA (output) + GPIOB (input) |
| Behavior | Sequential blink, one LED at a time | Real-time button → LED mirroring |
| Timing | `delay(1000)` busy-wait between transitions | None — tight polling loop |
| New concept introduced | Output config: `MODER`/`OTYPER`/`OSPEEDR`/`PUPDR`/`ODR` | Input config: `MODER=00`, `PUPDR` pull-up, reading `IDR` |


---
