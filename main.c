#include "ADC_DRIVER.h"

int main(void)
{
    //channel 0 (pA0)
    ADC_Init(0);
    
    while(1)
    {
        // Read voltage from ADC
        float voltage = ADC_Read_Voltage();
        // delay it will loop on itself
        for(volatile int i = 0; i < 100000; i++);
    }
}