/*
 * adc.h
 *
 *  Created on: Dec 20, 2025
 *      Author: brajo
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc_init(uint8_t channel);
uint32_t adc_read(uint8_t channel);
void adc_start_continuous(uint8_t channel);
uint32_t adc_read_continuous(void);

#endif /* ADC_H_ */
