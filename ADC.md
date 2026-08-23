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
----
**Register Summary Table**

| Register | Base Address | Offset | Full Address | Used For |
|----------|--------------|--------|--------------|----------|
| **RCC->AHB1ENR** | 0x40023800 | 0x30 | 0x40023830 | Enable GPIOB clock |
| **RCC->APB2ENR** | 0x40023800 | 0x44 | 0x40023844 | Enable ADC1 clock |
| **GPIOB->MODER** | 0x40020400 | 0x00 | 0x40020400 | Set LED pins as outputs |
| **GPIOB->ODR** | 0x40020400 | 0x14 | 0x40020414 | Turn LEDs ON/OFF |
| **ADC->CCR** | 0x40012000 | 0x300 | 0x40012300 | Set ADC prescaler |
| **ADC1->CR2** | 0x40012000 | 0x08 | 0x40012008 | Enable ADC, start conversion |
| **ADC1->SQR3** | 0x40012000 | 0x34 | 0x40012034 | Select ADC channel |
| **ADC1->SMPR2** | 0x40012000 | 0x10 | 0x40012010 | Set sampling time |
| **ADC1->SR** | 0x40012000 | 0x00 | 0x40012000 | Check conversion complete |
| **ADC1->DR** | 0x40012000 | 0x4C | 0x4001204C | Read ADC value |

**1. RCC->AHB1ENR (GPIO Clock Enable)**

| Property | Value |
|----------|-------|
| **Base Address** | 0x40023800 |
| **Offset** | 0x30 |
| **Full Address** | 0x40023830 |
| **Type** | Read/Write |
| **Reset Value** | 0x00000000 |

**Bit Configuration**

| Bit | Name | Value | Description |
|:---:|:---|:---:|:---|
| 1 | **GPIOBEN** | **0** | GPIOB clock disabled |
| 1 | **GPIOBEN** | **1** | **GPIOB clock enabled ← SET THIS!** |
