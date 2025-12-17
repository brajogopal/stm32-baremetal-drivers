#include "stm32f030x8.h"


#define GPIOAEN     	(1U<<17)
#define PIN5        	(1U<<5)
#define LED_PIN     	PIN5
#define PIN6 			(1U<<6)
#define PUSH_BUTTON		PIN6


int main (void){

	RCC->AHBENR |= GPIOAEN;   //Enabling RCC clock access to GPIOA PORT

	//set PA5 as Output pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	//set PA6 as Input pin
	GPIOA->MODER &=~ ((1U<<12) | (1U<<13));

	while(1){
		if (GPIOA->IDR & PUSH_BUTTON){
		GPIOA->BSRR = LED_PIN;  // TO turn ON the LED
		} else{
		GPIOA->BSRR = (1U<<21); //TO turn OFF the LED
		}
	}
}
