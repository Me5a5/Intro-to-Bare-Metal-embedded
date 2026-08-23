#include "../MCAL/RCC/RCC.h"


void setup(){
    RCC_Init();

}

void loop(){
    
}

int main(void)
{
    setup();  //as you know the setups is called only once


    while (1)
    {
        loop();   //loop is called continously so it's in the while(1)
    }
}