/*
 * board_spi.c
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#include "board_spi.h"
#include "stm32f030x8.h"

#define		GPIOAEN				(1U<<17)

/* pin15 - PA5 - SPI1_SCK
 * pin16 - PA6 - SPI1_MISO
 * pin17 - PA7 - SPI1_MOSI
 *
 * pin14 - PA4 - SPI1_NSS/CS*/

void board_spi1_gpio_init(void){
	/*Enable clock access to GPIOA*/
	RCC->AHBENR |= GPIOAEN;

	/*-----high speed for PA5/6/7------*/
	GPIOA->OSPEEDR |= (3U<<10) | (3U<<12) | (3U<<14);

	/*Set PA5,PA6 & PA7 mode to alternate function*/
	GPIOA->MODER &= ~((3U<<10) | (3U<<12) | (3U<<14));
	GPIOA->MODER |= ((2U<<10) | (2U<<12) | (2U<<14));

	//set PA4 as a output pin
	GPIOA->MODER &= ~(3U<<8);
	GPIOA->MODER |= (1U<<8);

	/*Set PA5,PA6 & PA7 alternate function mode to AF0*/
	GPIOA->AFR[0] &= ~ (0xFU<<20);
	GPIOA->AFR[0] &= ~ (0xFU<<24);
	GPIOA->AFR[0] &= ~ (0xFU<<28);
}

void board_cs_enable(void){
	GPIOA->ODR &= ~(1U<<4); //PA4 pin LOW
}

void board_cs_disable(void){

	GPIOA->ODR |= (1U<<4); //PA4 pin HIGH

}

