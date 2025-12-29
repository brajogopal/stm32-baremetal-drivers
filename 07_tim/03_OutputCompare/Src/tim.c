/*
 * tim.c
 *
 *  Created on: 27-Dec-2025
 *      Author: brajo
 */
//PA8(AF2) --  TIM1_CH1

#include "stm32f030x8.h"


#define RCC_APB2ENR_TIM1EN		(1U<<11)
#define prescaler				800
#define auto_reload				10000

#define CR1_CEN					(1U<<0)
#define TIM1_CCMR1_OC1M			(3U<<4)
#define TIM1_CCER_CC1E			(1U<<0)

#define RCC_APB1ENR_TIM3EN		(1U<<1)
#define TIM3_CCMR1_CC1S			(1U<<0)
#define TIM3_CCER_CC1E			(1U<<0)

#define GPIOAEN       			(1U<<17)


void tim1_1hz_init(void){
	/*Enable clock access to timer1*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	/*set prescaler value*/
	TIM1->PSC = prescaler -1;

	/*set auto-reload value*/
	TIM1->ARR = auto_reload -1;

	/*Clear Counter*/
	TIM1->CNT = 0;

	/*Enable timer*/
	TIM1->CR1 |= CR1_CEN;
}

void tim1_pa8_output_compare(void){

	/*******Setting the PA8 as Tim1_ch1  *******/
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER &= ~(3U<<16);
	GPIOA->MODER |= (2U<<16);
	GPIOA->AFR[1] &= ~(0xFU<<0);
	GPIOA->AFR[1] |= (2U<<0);

	/*Enable clock access to timer1*/
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	/*set prescaler value*/
	TIM1->PSC = prescaler -1;

	/*set auto-reload value*/
	TIM1->ARR = auto_reload -1;
	TIM1->CCR1 = auto_reload / 2;	//

	TIM1->CCMR1 &= ~(7U << 4);		//clear output compare toggle mode
	TIM1->CCMR1 |= TIM1_CCMR1_OC1M; //set output compare toggle mode

	TIM1->CCER |= TIM1_CCER_CC1E; 	//Enable tim1 ch1 in compare mode

	TIM1->BDTR |= (1U << 15);    // MOE (MANDATORY)

	TIM1->EGR  |= (1U << 0);     // UG

	/*Clear Counter*/
	TIM1->CNT = 0;

	/*Enable timer*/
	TIM1->CR1 |= CR1_CEN;
}

void tim3_pa6_input_capture(void){
	/*******Setting the PA6 as Tim3_ch1  *******/
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER &= ~(3U<<12);
	GPIOA->MODER |= (2U<<12);
	GPIOA->AFR[0] &= ~(0xFU<<24);
	GPIOA->AFR[0] |= (1U<<24);

	/*Enable clock access to timer3*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	/*set prescaler value*/
	TIM3->PSC = 8000 - 1;			// TIM3 tick = 1 ms (8 MHz / 8000)

	/*set CH1 to capture at rising edge*/
	TIM3->CCER &= ~(1U << 1);  		// CC1P = 0 → rising edge

	TIM3->CCER &= ~TIM3_CCER_CC1E;	// Capture disabled
	TIM3->CCMR1 &= ~(3U << 0);  	// clear CC1S
	TIM3->CCMR1 |= TIM3_CCMR1_CC1S;	// channel is configured as input
	TIM3->CCER |= TIM3_CCER_CC1E;	// Capture enabled

	TIM3->EGR |= (1U << 0); 	// Without this First few captures may have wrong timing
	TIM3->CNT = 0;				//Clear Counter
	TIM3->SR &= ~(1U << 1);  	// Clear CC1IF

	TIM3->CR1 |= CR1_CEN;	//Counter enabled
}
