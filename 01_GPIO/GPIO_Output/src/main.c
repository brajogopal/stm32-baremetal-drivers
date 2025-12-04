#include "stm32f030x8.h"

#define GPIOAEN               (1U<<17) // IS LIKE 0b 0000 0000 0000 0010 0000 0000 0000 0000

#define PIN6                  (1U<<6)
#define LED_PIN               (PIN6)


int main(void){

	 RCC->AHBENR |= GPIOAEN ;
	 GPIOA->MODER &= ~(1U<<13);
	 GPIOA->MODER |= (1U<<12);
	while(1){
		GPIOA->ODR ^= LED_PIN;
		for(int i= 0; i<100000 ; i++){}
	}
}
