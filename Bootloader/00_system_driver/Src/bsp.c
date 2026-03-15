/*
 * bsp.c
 *
 *  Created on: 01-Mar-2026
 *      Author: BROJOGOPAL
 */

#include "bsp.h"
#include "stm32f030x8.h"




	static GPIO_TypeDef *gpio_ports[] =
	{
	    GPIOA,
	    GPIOB,
	    GPIOC,
	    GPIOD
	};

	static const uint32_t gpio_clocks[] =
	{
	    RCC_AHBENR_GPIOAEN,		//(1U<<17)
	    RCC_AHBENR_GPIOBEN,		//(1U<<18)
	    RCC_AHBENR_GPIOCEN,		//(1U<<19)
		RCC_AHBENR_GPIODEN		//(1U<<20)
	};

	void pinMode(pin_t pin, mode_t mode)
	{
	    uint8_t port = PIN_PORT(pin);
	    uint8_t number = PIN_NUMBER(pin);

	    GPIO_TypeDef *gpio = gpio_ports[port];

	    RCC->AHBENR |= gpio_clocks[port];

	    gpio->MODER &= ~(3U << (number * 2));
	    gpio->MODER |=  (mode << (number * 2));
	}

void digitalWrite(pin_t pin, uint8_t state)
{
    uint8_t port = PIN_PORT(pin);
    uint8_t number = PIN_NUMBER(pin);

    GPIO_TypeDef *gpio = gpio_ports[port];

    if(state)
        gpio->BSRR = (1U << number);
    else
        gpio->BSRR = (1U << (number + 16));
}

uint8_t digitalRead(pin_t pin)
{
    uint8_t port = PIN_PORT(pin);
    uint8_t number = PIN_NUMBER(pin);

    GPIO_TypeDef *gpio = gpio_ports[port];

    return (gpio->IDR & (1U << number)) ? 1 : 0;
}

void togglePin(pin_t pin)
{
    uint8_t port = PIN_PORT(pin);
    uint8_t number = PIN_NUMBER(pin);

    GPIO_TypeDef *gpio = gpio_ports[port];

    gpio->ODR ^= (1U << number);
}

