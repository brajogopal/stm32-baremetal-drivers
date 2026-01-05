/*
 * tim.h
 *
 *  Created on: 27-Dec-2025
 *      Author: brajo
 */

#ifndef TIM_H_
#define TIM_H_

#define TIM1_SR_UIF		(1U<<0)
#define TIM3_SR_UIF		(1U<<0)

void tim1_1hz_init(void);
void tim3_1hz_interrupt_init(void);


#endif /* TIM_H_ */
