#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <stm32f401xc.h>

#define SET_BIT(reg, BIT)      (reg |= (1 << BIT))
#define CLEAR_BIT(reg, BIT)    (reg & ~(1 << BIT))
#define READ_BIT(reg, BIT)     ((reg >> BIT) & 1)

void ADC_Init(uint8_t channel); //unit8_t is 8 bits 
uint16_t ADC_Read_Raw(void);  //it returns 16 bits which is the nearest thing to 12 bits of the ADC without taking so much memory
float ADC_Read_Voltage(void);

#endif;
