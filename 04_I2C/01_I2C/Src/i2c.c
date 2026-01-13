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
#define 	ISR_BUSY		(1U<<15)
#define		CR2_START		(1U<<13)
#define		ISR_TXIS		(1U<<1)
#define		ISR_TC			(1U<<6)
#define		ISR_RXNE		(1U<<2)
#define 	RD_WRN			(1U<<10)
#define		CR2_AUTOEND		(1U<<25)


void I2C1_init (void){
	RCC->AHBENR |= GPIOBEN;		//Enable clock access to GPIOB

    /* 2. Set PB8 & PB9 to Alternate Function Mode (10) */
    GPIOB->MODER &= ~((3U << 16) | (3U << 18));  // Clear mode bits for PB8 & PB9
    GPIOB->MODER |=  ((2U << 16) | (2U << 18));  // Set 10 (AF mode) for both pins
    
    /*AFRH Configured for PB8 & PB9*/
    GPIOB->AFR[1] &= ~((0xF << 0) | (0xF << 4));  // Clear AFRH bits for PB8, PB9
    GPIOB->AFR[1] |=  ((1U  << 0) | (1U  << 4));  // AF1 for PB8, PB9

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


void I2C1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t *data)
{
    /* Wait until I2C bus is idle (no ongoing transfer) */
    while (I2C1->ISR & ISR_BUSY);


    /*------- START + WRITE phase: send register (memory) address-------- */
    I2C1->CR2 = (saddr << 1) | (1U << 16) | (0U << 10); // Slave address | NBYTES = 1 | Write direction

    I2C1->CR2 |= CR2_START;

    while (!(I2C1->ISR & ISR_TXIS)); //Wait until TXDR is ready and protocol allows data write (TXIS)

    I2C1->TXDR = maddr;				// send memory address

    while (!(I2C1->ISR & ISR_TC));	//Wait until write transfer is complete (restart allowed)


    /*--------RESTART + READ phase: receive data byte----*/
    I2C1->CR2 = (saddr << 1) | (1U << 16) | RD_WRN |  CR2_AUTOEND; // Slave address | NBYTES = 1 | Read | Auto STOP

    I2C1->CR2 |= CR2_START;

    while (!(I2C1->ISR & ISR_RXNE)); //Wait until received data is available in RXDR

    *data++ = I2C1->RXDR;				// Read received byte from RXDR
}


void I2C1_multiByteRead(uint8_t saddr, uint8_t maddr, uint8_t *data, uint8_t len)
{
    /* Wait until I2C bus is idle (no ongoing transfer) */
    while (I2C1->ISR & ISR_BUSY);


    /*------- START + WRITE phase: send register (memory) address-------- */
    I2C1->CR2 = (saddr << 1) | (1U << 16) | (0U << 10); // Slave address | NBYTES = 1 | Write direction

    I2C1->CR2 |= CR2_START;

    while (!(I2C1->ISR & ISR_TXIS)); //Wait until TXDR is ready and protocol allows data write (TXIS)

    I2C1->TXDR = maddr;				// send memory address

    while (!(I2C1->ISR & ISR_TC));	//Wait until write transfer is complete (restart allowed)


    /*--------RESTART + READ phase: receive data byte----*/
    I2C1->CR2 = (saddr << 1) | (len << 16) | RD_WRN |  CR2_AUTOEND; // Slave address | NBYTES = 1 | Read | Auto STOP

    I2C1->CR2 |= CR2_START;

    /* Read data bytes */
    for (uint8_t i = 0; i < len; i++)
    {
        /* Wait until a byte is received */
        while (!(I2C1->ISR & ISR_RXNE));

        /* Read received byte */
        data[i] = I2C1->RXDR;
    }
}


void I2C1_multiByteWrite(uint8_t saddr, uint8_t maddr, uint8_t *data, uint8_t len)
{
    /* Wait until I2C bus is idle (no ongoing transfer) */
    while (I2C1->ISR & ISR_BUSY);


    /*------- START + WRITE phase: send register (memory) address-------- */
    I2C1->CR2 = (saddr << 1) | ((len + 1) << 16) | CR2_AUTOEND; // Slave address | NBYTES = register + data | Auto STOP
    I2C1->CR2 |= CR2_START;

    while (!(I2C1->ISR & ISR_TXIS)); //Wait until TXDR is ready and protocol allows data write (TXIS)
    I2C1->TXDR = maddr;				// send memory address

    /* write data bytes */
    for (uint8_t i = 0; i < len; i++)
    {
        /* Wait until a byte is received */
        while (!(I2C1->ISR & ISR_TXIS));

        /* write data byte */
       I2C1->TXDR = data[i] ;
    }
}
