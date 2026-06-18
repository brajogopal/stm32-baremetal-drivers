/*
 * delay.c
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */


#include "stm32f030x8.h"
#include "delay.h"


#define APB2ENR_TIM1EN		(1U<<11)
#define prescaler				8000
#define TIM1_CR1_CEN			(1U<<0)


void delay_ms(uint32_t tim){
	/*Enable clock access to timer1*/
	RCC->APB2ENR |= APB2ENR_TIM1EN;

	/*set prescaler value*/
	TIM1->PSC = prescaler -1;

	/*set auto-reload value*/
	TIM1->ARR = tim -1;

    /* Force update to load PSC and ARR */
    TIM1->EGR |= (1U << 0);   // UG

	/*Clear Counter*/
	TIM1->CNT = 0;
	TIM1->SR  &= ~(1U << 0);  // Clear UIF


	/*Enable timer*/
	TIM1->CR1 |= TIM1_CR1_CEN;

    while (!(TIM1->SR & (1U << 0))) {
        // wait until UIF set
    }

    TIM1->CR1 &= ~TIM1_CR1_CEN;
}

void delay(uint32_t sec){
	int tim = sec * 1000;
	delay_ms(tim);
}
