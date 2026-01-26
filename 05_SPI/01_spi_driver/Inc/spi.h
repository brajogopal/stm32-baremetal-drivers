/*
 * spi.h
 *
 *  Created on: 14-Jan-2026
 *      Author: brajo
 */

#ifndef SPI_H_
#define SPI_H_
#include "stm32f030x8.h"

void spi1_gpio_init(void);
void spi1_config(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);
void cs_enable(void);
void cs_disable(void);


#endif /* SPI_H_ */
