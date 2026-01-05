#include "stm32f103x6.h"

#define RCC_APB2ENR_IOPCEN		(1U << 4)
#define GPIOC_CRH_RESET13		(0xFU << 20)
#define GPIOC_CRH_MODE13		(2U << 20)

int main (void){

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~GPIOC_CRH_RESET13;
	GPIOC->CRH |= GPIOC_CRH_MODE13;

	while(1){
		GPIOC->ODR ^= (1U<<13);
		for(int i=0 ;i<100000; i++){}
	}
}
