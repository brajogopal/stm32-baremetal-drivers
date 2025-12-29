/*
 * systick.c
 *
 *  Created on: 24-Dec-2025
 *      Author: brajo
 */
// Systick->LOAD = N (for delay)
// Delay = N/SYSCLK
//
#include "stm32f030x8.h"

#define SYSTICK_LOAD_VAL		8000
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSRC				(1U<<2)
#define CTRL_COUNTFLAG			(1U<<16)

void systickDelayMs(int delay){

	/*Reload with number of clock per ms*/
	SysTick->LOAD = SYSTICK_LOAD_VAL - 1;

	/*Clear systick current value register*/
	SysTick->VAL = 0;

	/*Enable systick and select internal clock src*/
	SysTick->CTRL |= CTRL_ENABLE | CTRL_CLKSRC;

	for(int i=0; i<delay; i++){

		/*Wait until the counterflag is set*/
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}
	SysTick->CTRL = 0;
}

