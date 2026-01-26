/*
 * spi.c
 *
 *  Created on: 14-Jan-2026
 *      Author: brajo
 */
#include "spi.h"


#include "stm32f030x8.h"


/*-------CR1 bits------*/
#define 	CR1_BR				(2U<<3)// 8MHZ/8 = 1MHZ SPI clock
#define 	CR1_SPE				(1U<<6)

/*---------SR bits--------*/
#define		SR_RXNE				(1U<<0)
#define		SR_TXE				(1U<<1)
#define		SR_BSY				(1U<<7)
#define		APB2ENR_SPI1EN		(1U<<12)





void spi1_transmit(uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {

    	/*Wait for TX-Buffer to be Empty*/
        while (!(SPI1->SR & SPI_SR_TXE));

        /*Write the data to the data register*/
        *(volatile uint8_t *)&SPI1->DR = data[i];
    }
    /*wait for BUSY flag to reset*/
    while (SPI1->SR & SPI_SR_BSY);

	/*Clear OVR Flag by reading SR & DR*/
	volatile uint8_t temp = SPI1->DR;
	temp = SPI1->SR;
	(void)temp;
}

void spi1_receive(uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        *(volatile uint8_t *)&SPI1->DR = 0xFF;	//Send dummy data

        while (!(SPI1->SR & SPI_SR_RXNE));		//Wait for RXNE Flag to be set

        data[i] = *(volatile uint8_t *)&SPI1->DR;//Read data from data register
    }
}

