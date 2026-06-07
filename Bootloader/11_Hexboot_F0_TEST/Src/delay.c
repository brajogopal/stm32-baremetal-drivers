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

volatile uint32_t sys_tick = 0;

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

void SysTick_Handler(void)
{
    sys_tick++;
}

void systick_delay(uint32_t ms)
{
    uint32_t start = sys_tick;
    while ((sys_tick - start) < ms);
}

void systick_init(void)
{
    SysTick->LOAD = 48000 - 1;   // 1ms (for 48MHz clock)
    SysTick->VAL  = 0;

    SysTick->CTRL =
        (1 << 2) |   // CLKSOURCE = CPU
        (1 << 1) |   // TICKINT = enable interrupt
        (1 << 0);    // ENABLE
}



