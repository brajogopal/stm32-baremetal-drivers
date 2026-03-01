/*
 * bsp.c
 *
 *  Created on: 01-Mar-2026
 *      Author: BROJOGOPAL
 */

#include "bsp.h"
#include "stm32f030x8.h"


typedef enum {
    INPUT  = 0x0,
    OUTPUT = 0x1,
    AF     = 0x2,
    PWM	   = 0x3
} mode_t;

typedef enum {
    GPIOA  = 0x0,
    GPIOB = 0x1,
    GPIOC  = 0x2,
    GPIOD = 0x3
} port_t;

void pinMode(GPIO_TypeDef *port,uint16_t pin, mode_t mode){

	RCC->AHBENR |=

	port->MODER &= ~(3U << (pin * 2));   // clear bits
    if (mode == INPUT) {
        // configure as input
    	 port->MODER |= (0U << (pin * 2));
    } else if (mode == OUTPUT) {
        // configure as output
    	 port->MODER |= (1U << (pin * 2));
    } else if (mode == AF) {
        // configure alternate function
    	 port->MODER |= (2U << (pin * 2));
    } else if (mode == PWM) {
        // configure alternate function
    	 port->MODER |= (3U << (pin * 2));
    }
}

