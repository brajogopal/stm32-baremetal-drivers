/*
 * exti.c
 *
 *  Created on: 30-Dec-2025
 *      Author: brajo
 */

#include "exti.h"

#define GPIOC_EN		(1U<<19)
#define SYSCFG_EN		(1U<<0)



void pc13_exti_init(void){

	/*Disable global interrupts*/
	__disable_irq();

	/*Enable clock access for GPIOC*/
	RCC->AHBENR |= GPIOC_EN;

	/*Set PC13 as input*/
	GPIOC->MODER &= ~(3U<<26);
	GPIOC->PUPDR &= ~(3U << 26);
	GPIOC->PUPDR |=  (1U << 26);   // Pull-up

	/*Enable clock access for SYSCFG*/
	RCC->APB2ENR |= SYSCFG_EN;

	/*Select PORTC for EXTI13*/
	SYSCFG->EXTICR[3] &= ~(0xFU << 4);   // CLEAR EXTI13 bits
	SYSCFG->EXTICR[3] |= (2U<<4);

	/*Unmask EXTI13*/
	EXTI->IMR |= (1U<<13);

	/*Select falling edge trigger*/
	EXTI->FTSR |= (1U<<13);

	EXTI->PR |= (1U << 13);   // Clear pending flag

	/*Enable EXTI13 line in NVIC*/
	__NVIC_EnableIRQ(EXTI4_15_IRQn);

	/*Enable global interrupts*/
	__enable_irq();
}
