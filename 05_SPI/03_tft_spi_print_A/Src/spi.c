/*
 * spi.c
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#include "spi.h"
#include "spi_reg.h"

#define		APB2ENR_SPI1EN		(1U<<12)
volatile uint8_t temp;

void spi1_init(void){
	/*Enable clock access to SPI1 module*/
	RCC->APB2ENR |= APB2ENR_SPI1EN;

	SPI1->CR1 = 0;

	/*Set MSTR = 1 → Master mode*/
	SPI1->CR1 |= (1U<<2);

	/*Set prescaler clock to fPCLK/4*/
	SPI1->CR1 |= CR1_BR;

	/*Select software slave management by setting SSM=1 and SSI=1*/
	SPI1->CR1 |= (1U<<8);
	SPI1->CR1 |= (1U<<9);

	/*Set Data Size for 8bit */
	SPI1->CR2 = 0;
	SPI1->CR2 |= (7U<<8);

	/*Enable SPI Communication*/
	SPI1->CR1 |= CR1_SPE;
}


void spi1_transmit(uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {

    	/*Wait for TX-Buffer to be Empty*/
        while (!(SPI1->SR & SPI_SR_TXE));

        /*Write the data to the data register*/
        *(volatile uint8_t *)&SPI1->DR = data[i];
    }
    /*wait for BUSY flag to reset*/
    while (SPI1->SR & SPI_SR_BSY){};

	/*Clear OVR Flag by reading SR & DR*/
	temp = SPI1->DR;
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

