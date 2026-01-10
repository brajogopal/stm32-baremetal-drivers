/*
 * rcc_clock.h
 *
 *  Created on: 08-Jan-2026
 *      Author: brajo
 */

#ifndef RCC_CLOCK_H_
#define RCC_CLOCK_H_

#include <stdint.h>

uint32_t get_sws(void);
uint32_t get_hpre(void);
uint32_t get_ppre(void);
uint32_t get_hclk_div(void);
uint32_t get_pclk_div(void);
uint32_t rcc_get_pclk1_freq(void);



#endif /* RCC_CLOCK_H_ */
