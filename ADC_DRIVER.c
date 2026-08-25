#include "ADC_DRIVER.h"

void ADC_Init(uint8_t channel)
{
    // sets the 8th bit to 1 to enable ADC clock 
    SET_BIT(RCC->APB2ENR, 8);
    
    // 2. Set prescaler (ADC->CCR bits 17:16 = 01)
    SET_BIT( ADC->CCR,16);
    CLEAR_BIT(;ADC->CCR, 17)
    
    // (ADC1->SQR1 bits 23:20 = 0000)
    ADC1->SQR1 &= ~(0xF << 20);    // Clear bits 23:20
    ADC1->SQR1 |= (0 << 20);        // Set bits 23:20 to 0000
    
    // 4. Select ADC channel (ADC1->SQR3 bits 4:0 = channel)
    ADC1->SQR3 &= ~(0x1F << 0);    // Clear bits 4:0
    ADC1->SQR3 |= (channel << 0);  // Set bits 4:0 to channel number
    /*
    0x1F << 0	Creates a mask: 0b11111 << 0 = bits 0,1,2,3,4
    ~(0x1F << 0)	Inverts it: all bits = 1, except bits 0-4 = 0
    ADC1->SQR3 &= ~(...)	Clears bits 0-4 to 0 (preserves other bits)
    ADC1->SQR3 |= (channel << 0)	Sets bits 0-4 to the channel number
    */
    
    // 5. Set sampling time to 480 cycles (ADC1->SMPR2 bits 2:0 = 111)
    if(channel <= 9)
        ADC1->SMPR2 |= (7 << (3 * channel));
    else
        ADC1->SMPR1 |= (7 << (3 * (channel - 10)));
    
    // 6. Configure: Single mode (CONT=0), Right alignment (ALIGN=0)
    ADC1->CR2 &= ~(1U << 12);    // CONT = 0
    ADC1->CR2 &= ~(1U << 11);    // ALIGN = 0
    
    // 7. Enable ADC (ADON=1)
    ADC1->CR2 |= (1U << 0);
}

uint16_t ADC_Read_Raw(void)
{
    // Start conversion (SWSTART=1)
    ADC1->CR2 |= (1U << 31);
    
    // Wait for EOC flag
    while(!(ADC1->SR & (1U << 1)));
    
    // Read and return data
    return ADC1->DR;
}

float ADC_Read_Voltage(void)
{
    uint16_t raw = ADC_Read_Raw();
    return (raw * 3.3f) / 4095.0f;
}