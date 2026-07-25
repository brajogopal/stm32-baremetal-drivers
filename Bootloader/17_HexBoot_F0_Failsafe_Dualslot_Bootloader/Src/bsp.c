/*
 * bsp.c
 *
 * Board Support Package.
 */

#include "bsp.h"
#include "stm32f030x8.h"




void pinMode(pin_t pin, mode_t mode)
{
	uint8_t port = PIN_PORT(pin);
	uint8_t number = PIN_NUMBER(pin);

	GPIO_TypeDef *gpio;

	switch (port) {
	case 0:
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		gpio = GPIOA;
		break;

	case 1:
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		gpio = GPIOB;
		break;

	case 2:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		gpio = GPIOC;
		break;

	case 3:
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		gpio = GPIOD;
		break;

	default:
		return;
	}
	gpio->MODER &= ~(3U << (number * 2));
	gpio->MODER |= (mode << (number * 2));
}




void digitalWrite(pin_t pin, uint8_t state)
{
	uint8_t port = PIN_PORT(pin);
	uint8_t number = PIN_NUMBER(pin);

	GPIO_TypeDef *gpio;

	switch (port) {
	case 0:
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		gpio = GPIOA;
		break;

	case 1:
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		gpio = GPIOB;
		break;

	case 2:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		gpio = GPIOC;
		break;

	case 3:
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		gpio = GPIOD;
		break;

	default:
		return;
	}
	if (state)
		gpio->BSRR = (1U << number);
	else
		gpio->BSRR = (1U << (number + 16));
}



uint8_t digitalRead(pin_t pin)
{
	uint8_t port = PIN_PORT(pin);
	uint8_t number = PIN_NUMBER(pin);

	GPIO_TypeDef *gpio;

	switch (port) {
	case 0:
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		gpio = GPIOA;
		break;

	case 1:
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		gpio = GPIOB;
		break;

	case 2:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		gpio = GPIOC;
		break;

	case 3:
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		gpio = GPIOD;
		break;

	default:
		return 0;
	}
	return (gpio->IDR & (1U << number)) ? 1 : 0;
}



void togglePin(pin_t pin)
{
	uint8_t port = PIN_PORT(pin);
	uint8_t number = PIN_NUMBER(pin);
	GPIO_TypeDef *gpio;

	switch (port) {
	case 0:
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		gpio = GPIOA;
		break;

	case 1:
		RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
		gpio = GPIOB;
		break;

	case 2:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		gpio = GPIOC;
		break;

	case 3:
		RCC->AHBENR |= RCC_AHBENR_GPIODEN;
		gpio = GPIOD;
		break;

	default:
		return;
	}

	gpio->ODR ^= (1U << number);
}

