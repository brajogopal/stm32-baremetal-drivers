/*
 * main.c
 *
 * Simple application demonstrating USART2 TX & RX
 */
#include 	<stdio.h>
#include 	"uart.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5



#define UART_BAUDRATE	115200
volatile char key;


int main (void){

	char message[31] = "Hello from stm32 DMA transfer\n\r";

    /* Initialize USART2 at 115200 baud */
    uart2_tx_init(UART_BAUDRATE);
    dma_uart2_tx_init((uint32_t) message,(uint32_t)&USART2->TDR, 31);

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~ (1U<<11);

	while(1){

	}
}
	static void dma_callback(void){
		GPIOA->ODR |=LED_PIN;
	}


	void DMA1_CH4_5_IRQHandler(void){
		//check for transfer complete interrupt
		if(DMA1->ISR & ISR_TCIF4){

			//Clear flag
			DMA1->IFCR |= IFCR_CTCIF4;

			//do something
			dma_callback();
		}
	}
