/*
 * adc.c
 *
 *  Created on: Dec 20, 2025
 *      Author: brajo
 */
#include "stm32f030x8.h"
#include "adc.h"


#define GPIOAEN     		(1U<<17)
#define ADC_EN				(1U<<9)

#define ADC_CR_ADEN			(1U<<0)
#define ADC_CR_ADSTART		(1U<<2)
#define ADC_CR_ADCAL   		(1U << 31)

#define ADC_CFGR1_CONT		(1U<<13)

#define IER_EOCIE			(1U<<2)

void adc_init(uint8_t channel){

	if (channel > 7) return;

	/****Configure the PA1 for ADC****/
	RCC->AHBENR |= GPIOAEN;   			//Enabling RCC clock access to GPIOA PORT
	GPIOA->MODER &= ~(3U<<(channel * 2));
	GPIOA->MODER |= (3U<<(channel * 2)); 			//Set the mode of PA1 to analog mode

	/****Configure adc module****/
	RCC->APB2ENR |=  ADC_EN;			//enable clock access to adc module

    /* ADC calibration */
    ADC1->CR &= ~ADC_CR_ADEN;      		// ensure ADC disabled
    ADC1->CR |= ADC_CR_ADCAL;		  	// start calibration
    while (ADC1->CR & ADC_CR_ADCAL){};	// wait

	/*****configure adc parameters*****/
	ADC1->CR |= ADC_CR_ADEN;			//Enable Adc module

}

void adc_interrupt_init(uint8_t channel){

	if (channel > 7) return;

	/****Configure the PA1 for ADC****/
	RCC->AHBENR |= GPIOAEN;   			//Enabling RCC clock access to GPIOA PORT
	GPIOA->MODER &= ~(3U<<(channel * 2));
	GPIOA->MODER |= (3U<<(channel * 2)); 			//Set the mode of PA1 to analog mode

	/****Configure adc module****/
	RCC->APB2ENR |=  ADC_EN;			//enable clock access to adc module

	ADC1->IER |= IER_EOCIE;				//Enable ADC End OF Conversion Interrupt

	__NVIC_EnableIRQ(ADC1_IRQn);		//Enable ADC Interrupt in NVIC

    /* ADC calibration */
    ADC1->CR &= ~ADC_CR_ADEN;      		// ensure ADC disabled
    ADC1->CR |= ADC_CR_ADCAL;		  	// start calibration
    while (ADC1->CR & ADC_CR_ADCAL){};	// wait

	/*****configure adc parameters*****/
	ADC1->CR |= ADC_CR_ADEN;			//Enable Adc module

}



uint32_t adc_read(uint8_t channel)
{
	if (channel > 15) return 0;
    ADC1->CHSELR = (1U << channel);   // select one channel
    ADC1->CR |= ADC_CR_ADSTART;       // start conversion
    while (!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}


void adc_start_continuous(uint8_t channel)
{
	if (channel > 15) return;

    /* Select channel once */
    ADC1->CHSELR = (1U << channel);

    /* Enable continuous mode */
    ADC1->CFGR1 |= ADC_CFGR1_CONT;

    /* Start conversion ONCE */
    ADC1->CR |= ADC_CR_ADSTART;
}


uint32_t adc_read_continuous(void)
{
    /* Wait for new conversion */
    while (!(ADC1->ISR & ADC_ISR_EOC));

    /* Read latest converted value */
    return ADC1->DR;
}


