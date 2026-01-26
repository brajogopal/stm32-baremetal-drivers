/*
 * spi.h
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */


#ifndef SPI_H_
#define SPI_H_
#include 	<stdint.h>

void spi1_init(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);



#endif /* SPI_H_ */
