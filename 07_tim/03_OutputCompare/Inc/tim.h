/*
 * tim.h
 *
 *  Created on: 27-Dec-2025
 *      Author: brajo
 */

#ifndef TIM_H_
#define TIM_H_

#define TIM1_SR_UIF		(1U<<0)
#define TIM3_SR_CC1IF	(1U<<1)

void tim1_1hz_init(void);
void tim1_pa8_output_compare(void);
void tim3_pa6_input_capture(void);

#endif /* TIM_H_ */
