/*
 * i2c.c
 *
 *  Created on: 07-Jan-2026
 *      Author: brajo
 */
//PINOUT ->  45-PB8(AF1)-SCL, 46-PB9(AF1)-SDA

#include "stm32f030x8.h"
#include "i2c.h"
#include "rcc_clock.h"

#define 	GPIOBEN			(1U<<18)
#define		I2C1EN			(1U<<21)





void I2C1_init (void){
	RCC->AHBENR |= GPIOBEN;		//Enable clock access to GPIOB

    /* 2. Set PB8 & PB9 to Alternate Function Mode (10) */
    GPIOB->MODER &= ~((3U << 16) | (3U << 18));  // Clear mode bits for PB8 & PB9
    GPIOB->MODER |=  ((2U << 16) | (2U << 18));  // Set 10 (AF mode) for both pins

	//set PB8 and PB9 output type to open drain
    GPIOB->OTYPER |= ((1U<<8) | (1U<<9));

	//Enable Pullup for PB8 and PB9
    GPIOB->PUPDR &= ~((3U << 16) | (3U << 18)); 	// Clear PUPDR bits for PB8 & PB9
    GPIOB->PUPDR |=  ((1U << 16) | (1U << 18));		// set PB8 & PB9 in Pullup mode

	//Enable clock access to I2C1
    RCC->APB1ENR |= I2C1EN;


    /*Configure the i2c1*/
    RCC->APB1RSTR |= (1U<<21);	//Enter Reset MODE

    RCC->APB1RSTR &= ~(1U<<21);	//Come out of Reset MODE

    I2C1->CR1 &= ~(1U<<0);

    I2C1->TIMINGR = i2c_calc_timing(100000); // 100 kHz

    I2C1->CR1 |= (1U<<0);		//enable i2c1 module
}
