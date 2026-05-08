/*
 * delay.h
 *
 *  Created on: 31-Jan-2026
 *      Author: Brajo
 */

#ifndef DELAY_H_
#define DELAY_H_

void delay_ms(uint32_t tim);
void delay(uint32_t sec);


void systick_delay(uint32_t ms);
void systick_init(void);
void SysTick_Handler(void);

#endif /* DELAY_H_ */
