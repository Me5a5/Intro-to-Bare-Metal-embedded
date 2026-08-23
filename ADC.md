**STM32F401CC: ADC Peripheral Documentation**

**Reference Manual:** RM0368 (STM32F401xB/C and STM32F401xD/E)
**Microcontroller:** STM32F401CC
**Peripheral:** ADC1

**1. Introduction**
The STM32F401CC uses a 12-bit successive approximation ADC (ADC1) to read analog voltages from 0V to 3.3V.

**Peripheral Base Addresses**
- **GPIOA Base Address:** <mark>0x4002 0000</mark>
- **RCC Base Address:** <mark>0x4002 3800</mark>
- **ADC1 Base Address:** <mark>0x4001 2000</mark>

---

**2. Pin Configuration**
- **PA0** is the ADC input (Analog Mode).
- **PA1, PA2, PA3** are the LED outputs (Push-Pull Output Mode).

**RCC_AHB1ENR**
- **Base Address:** <mark>0x4002 3800</mark>
- **Offset:** <mark>0x30</mark>
- **Actual Address:** <mark>0x4002 3830</mark>
- **Reset Value:** <mark>0x0000 0000</mark>
- **Usage:** Sets <mark>RCC->AHB1ENR</mark> Bit 0 to <mark>1</mark> to enable the clock for GPIO Port A.

**2.2. GPIOA_MODER**
- **Base Address:** <mark>0x4002 0000</mark>
- **Offset:** <mark>0x00</mark>
- **Actual Address:** <mark>0x4002 0000</mark>
- **Reset Value:** <mark>0xA800 0000</mark>
- **Usage:** 
  - Sets PA0 to Analog Mode, we set MODER bits <mark>11</mark>.
  - Sets PA1, PA2, PA3 to Output Mode, we set <mark>MODER</mark> bits to <mark>01</mark>

 **2.3. GPIOA_OTYPER**
- **Base Address:** <mark>0x4002 0000</mark>
- **Offset:** <mark>0x04</mark>
- **Actual Address:** <mark>0x4002 0004</mark>
- **Reset Value:** <mark>0x0000 0000</mark>
- **Usage:** Sets <mark>OTYPER</mark> bits 1, 2, 3 to <mark>0</mark> to configure the LEDs in Push-Pull mode.

