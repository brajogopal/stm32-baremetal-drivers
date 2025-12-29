/*
 * tim.c
 *
 *  Created on: 27-Dec-2025
 *      Author: brajo
 */


#include "stm32f030x8.h"


#define RCC_APB2ENR_TIM1EN		(1U<<11)
#define prescaler				800
#define auto_reload				10000
#define TIM1_CR1_CEN			(1U<<0)


void tim1_1hz_init(void){
	/*Enable clock access to timer1*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	/*set prescaler value*/
	TIM1->PSC = prescaler -1;

	/*set auto-reload value*/
	TIM1->ARR = auto_reload -1;

    /* Force update to load PSC and ARR */
    TIM1->EGR |= (1U << 0);   // UG

	/*Clear Counter*/
	TIM1->CNT = 0;
	TIM1->SR  &= ~(1U << 0);  // Clear UIF


	/*Enable timer*/
	TIM1->CR1 |= TIM1_CR1_CEN;

}
