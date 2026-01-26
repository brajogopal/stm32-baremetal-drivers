/*
 * spi_reg.h
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#ifndef SPI_REG_H_
#define SPI_REG_H_


#include "stm32f030x8.h"


/*-------CR1 bits------*/
#define 	CR1_BR				(2U<<3)// 8MHZ/8 = 1MHZ SPI clock
#define 	CR1_SPE				(1U<<6)

/*---------SR bits--------*/
#define		SR_RXNE				(1U<<0)
#define		SR_TXE				(1U<<1)
#define		SR_BSY				(1U<<7)

#endif /* SPI_REG_H_ */
