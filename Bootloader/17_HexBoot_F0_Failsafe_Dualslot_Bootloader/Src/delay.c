/*
 * delay.c
 *
 * Timer-based delay driver.
 */


#include "stm32f030x8.h"
#include "delay.h"


#define APB2ENR_TIM1EN	(1U<<11)
#define TIM1_PRESCALER	8000U
#define TIM1_CR1_CEN	(1U<<0)
#define TIM_EGR_UG    (1U << 0)
#define TIM_SR_UIF    (1U << 0)


void delay_ms(uint32_t ms)
{
	/*Enable clock access to timer1*/
	RCC->APB2ENR |= APB2ENR_TIM1EN;

	/*set prescaler value*/
	TIM1->PSC = TIM1_PRESCALER -1;

	/*set auto-reload value*/
	TIM1->ARR = ms -1;

    /* Force update to load PSC and ARR */
    TIM1->EGR |= TIM_EGR_UG;   // UG

	/*Clear Counter*/
	TIM1->CNT = 0;
	TIM1->SR  &= ~TIM_SR_UIF;  // Clear UIF


	/*Enable timer*/
	TIM1->CR1 |= TIM1_CR1_CEN;

    while (!(TIM1->SR & TIM_SR_UIF)) {
        // wait until UIF set
    }

    TIM1->CR1 &= ~TIM1_CR1_CEN;
}

void delay(uint32_t seconds)
{
	uint32_t ms = seconds * 1000U;
	delay_ms(ms);
}
