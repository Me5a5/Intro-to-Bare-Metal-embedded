/**
 ******************************************************************************
 * @file    rcc_direct.h
 * @brief   Enable any peripheral clock with ONE function, using nothing but
 *          raw register addresses - no struct, no RCC_Init(), no PLL.
 *
 *          STM32F401CC boots on HSI (16 MHz internal RC) by default, and
 *          every AHB1/AHB2/APB1/APB2 bus clock is already valid off that
 *          HSI. The only thing standing between reset and "peripheral is
 *          usable" is its one enable bit in the matching RCC_xxENR
 *          register - this function just sets that bit at that address.
 *
 *          Reference: RM0368, RCC register map (base 0x40023800)
 ******************************************************************************
 */
#ifndef RCC_DIRECT_H
#define RCC_DIRECT_H

#include <stdint.h>

/* -------------------------------------------------------------------------
 * RCC clock-enable register addresses (nothing but addresses, as asked)
 * ---------------------------------------------------------------------- */
#define RCC_AHB1ENR   0x40023830UL   /* GPIOA..H, DMA1, DMA2, ...        */
#define RCC_AHB2ENR   0x40023834UL   /* USB OTG FS, ...                  */
#define RCC_APB1ENR   0x40023840UL   /* TIM2-5, I2C1-3, SPI2/3, PWR, ...  */
#define RCC_APB2ENR   0x40023844UL   /* TIM1, USART1/6, ADC1, SPI1, ...  */

/* -------------------------------------------------------------------------
 * Bit positions inside each register (pass one of these as `bit`)
 * ---------------------------------------------------------------------- */
#define RCC_AHB1ENR_GPIOAEN   0
#define RCC_AHB1ENR_GPIOBEN   1
#define RCC_AHB1ENR_GPIOCEN   2
#define RCC_AHB1ENR_GPIODEN   3
#define RCC_AHB1ENR_GPIOEEN   4
#define RCC_AHB1ENR_GPIOHEN   7
#define RCC_AHB1ENR_DMA1EN    21
#define RCC_AHB1ENR_DMA2EN    22

#define RCC_AHB2ENR_OTGFSEN   7

#define RCC_APB1ENR_TIM2EN    0
#define RCC_APB1ENR_TIM3EN    1
#define RCC_APB1ENR_TIM4EN    2
#define RCC_APB1ENR_TIM5EN    3
#define RCC_APB1ENR_SPI2EN    14
#define RCC_APB1ENR_SPI3EN    15
#define RCC_APB1ENR_USART2EN  17
#define RCC_APB1ENR_I2C1EN    21
#define RCC_APB1ENR_I2C2EN    22
#define RCC_APB1ENR_I2C3EN    23
#define RCC_APB1ENR_PWREN     28

#define RCC_APB2ENR_TIM1EN    0
#define RCC_APB2ENR_USART1EN  4
#define RCC_APB2ENR_USART6EN  5
#define RCC_APB2ENR_ADC1EN    8
#define RCC_APB2ENR_SDIOEN    11
#define RCC_APB2ENR_SPI1EN    12
#define RCC_APB2ENR_SPI4EN    13
#define RCC_APB2ENR_SYSCFGEN  14
#define RCC_APB2ENR_TIM9EN    16
#define RCC_APB2ENR_TIM10EN   17
#define RCC_APB2ENR_TIM11EN   18

/**
 * @brief  Turn on the bus clock for one peripheral. That's it - one
 *         read-modify-write to one address. Peripheral is usable right
 *         after this returns, still running on the default HSI 16 MHz.
 *
 * @param  enr_addr  address of the *ENR register, e.g. RCC_AHB1ENR
 * @param  bit       peripheral's enable bit, e.g. RCC_AHB1ENR_GPIOAEN
 */
static inline void RCC_EnableClock(uint32_t enr_addr, uint8_t bit)
{
    *(volatile uint32_t *)enr_addr |= (1UL << bit);
}

/* -------------------------------------------------------------------------
 * "Enable everything" masks - OR of every bit macro defined above for that
 * register. Only real, implemented bits are set - reserved bits are left
 * alone on purpose.
 * ---------------------------------------------------------------------- */
#define RCC_AHB1ENR_ALL  ( (1UL<<RCC_AHB1ENR_GPIOAEN) | (1UL<<RCC_AHB1ENR_GPIOBEN) | \
                            (1UL<<RCC_AHB1ENR_GPIOCEN) | (1UL<<RCC_AHB1ENR_GPIODEN) | \
                            (1UL<<RCC_AHB1ENR_GPIOEEN) | (1UL<<RCC_AHB1ENR_GPIOHEN) | \
                            (1UL<<RCC_AHB1ENR_DMA1EN)  | (1UL<<RCC_AHB1ENR_DMA2EN)  )

#define RCC_AHB2ENR_ALL  ( (1UL<<RCC_AHB2ENR_OTGFSEN) )

#define RCC_APB1ENR_ALL  ( (1UL<<RCC_APB1ENR_TIM2EN)   | (1UL<<RCC_APB1ENR_TIM3EN)   | \
                            (1UL<<RCC_APB1ENR_TIM4EN)   | (1UL<<RCC_APB1ENR_TIM5EN)   | \
                            (1UL<<RCC_APB1ENR_SPI2EN)   | (1UL<<RCC_APB1ENR_SPI3EN)   | \
                            (1UL<<RCC_APB1ENR_USART2EN) | (1UL<<RCC_APB1ENR_I2C1EN)   | \
                            (1UL<<RCC_APB1ENR_I2C2EN)   | (1UL<<RCC_APB1ENR_I2C3EN)   | \
                            (1UL<<RCC_APB1ENR_PWREN)    )

#define RCC_APB2ENR_ALL  ( (1UL<<RCC_APB2ENR_TIM1EN)   | (1UL<<RCC_APB2ENR_USART1EN) | \
                            (1UL<<RCC_APB2ENR_USART6EN) | (1UL<<RCC_APB2ENR_ADC1EN)   | \
                            (1UL<<RCC_APB2ENR_SDIOEN)   | (1UL<<RCC_APB2ENR_SPI1EN)   | \
                            (1UL<<RCC_APB2ENR_SPI4EN)   | (1UL<<RCC_APB2ENR_SYSCFGEN) | \
                            (1UL<<RCC_APB2ENR_TIM9EN)   | (1UL<<RCC_APB2ENR_TIM10EN)  | \
                            (1UL<<RCC_APB2ENR_TIM11EN)  )

/**
 * @brief  One call, every peripheral clock on. Enables the bus clock for
 *         every implemented peripheral on AHB1, AHB2, APB1 and APB2 in a
 *         single shot, so from this point on you can touch any peripheral's
 *         registers immediately.
 *
 *         There is nothing to wait for here: this never touches HSEON,
 *         PLLON, or the SW/SWS clock-switch bits, so no *RDY flag needs
 *         polling - the core is already running on HSI (16 MHz), which is
 *         live and stable the instant the chip comes out of reset. This
 *         function only ungates peripheral bus clocks, it does not change
 *         the system clock itself.
 *
 * @note   Trade-off: every enabled peripheral draws a little extra current
 *         even if you never use it. Fine for bring-up/prototyping; for a
 *         production build, prefer RCC_EnableClock() per-peripheral once
 *         you know what you actually use.
 */
static inline void RCC_Init(void)
{
    *(volatile uint32_t *)RCC_AHB1ENR |= RCC_AHB1ENR_ALL;
    *(volatile uint32_t *)RCC_AHB2ENR |= RCC_AHB2ENR_ALL;
    *(volatile uint32_t *)RCC_APB1ENR |= RCC_APB1ENR_ALL;
    *(volatile uint32_t *)RCC_APB2ENR |= RCC_APB2ENR_ALL;
}

#endif /* RCC_DIRECT_H */