/*
 * board_spi.h
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#ifndef BOARD_SPI_H_
#define BOARD_SPI_H_
#include 	<stdint.h>

void board_spi1_gpio_init(void);
void board_cs_enable(void);
void board_cs_disable(void);

#endif /* BOARD_SPI_H_ */
