#include "../MCAL/RCC/RCC.h"
#include "../Lib/BIT_MATH.h"
#define GPIOA_BASE_ADDRESS 0x40020000


#define GPIOOA_MODER_OFFSET  0x00
#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE_ADDRESS + GPIOOA_MODER_OFFSET))


#define GPIOA_OTYPER_OFFSET 0x04
#define GPIOA_OTYPER (*(volatile unsigned int *)(GPIOA_BASE_ADDRESS + GPIOA_OTYPER_OFFSET))

#define GPIOA_OSPEEDR_OFFSET 0x08
#define GPIOA_OSPEEDR (*(volatile unsigned int *)(GPIOA_BASE_ADDRESS + GPIOA_OSPEEDR_OFFSET))   

#define GPIOA_PUPDR_OFFSET 0x0C
#define GPIOA_PUPDR (*(volatile unsigned int *)(GPIOA_BASE_ADDRESS + GPIOA_PUPDR_OFFSET))

#define GPIOA_ODR_OFFSET 0x14
#define GPIOA_ODR (*(volatile unsigned int *)(GPIOA_BASE_ADDRESS + GPIOA_ODR_OFFSET))

void delay(volatile unsigned int count){
    while(count--);
}

void setup(){
    RCC_Init();
    int pin;
    for (pin = 0; pin < 3; pin++){
        SET_BIT(GPIOA_MODER, pin * 2);       // output
        CLEAR_BIT(GPIOA_MODER, pin * 2 + 1);

        CLEAR_BIT(GPIOA_OTYPER, pin);        // push-pull

        CLEAR_BIT(GPIOA_OSPEEDR, pin * 2);   // speed LOW
        CLEAR_BIT(GPIOA_OSPEEDR, pin * 2 + 1);

        CLEAR_BIT(GPIOA_PUPDR, pin * 2);     // pull-down
        SET_BIT(GPIOA_PUPDR, pin * 2 + 1);
}


}

void loop(){
    int pin;
    for(pin = 0; pin < 3; pin++){
        SET_BIT(GPIOA_ODR, pin); // Set PAx high
        delay(1000); // Delay for 1 second
        CLEAR_BIT(GPIOA_ODR, pin); // Set PAx low
        delay(1000); // Delay for 1 second
    }




    
}

int main(void)
{
    setup();  //as you know the setups is called only once


    while (1)
    {
        loop();   //loop is called continously so it's in the while(1)
    }
}