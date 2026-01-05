/*
 * main.c
 *
 * Simple application demonstrating USART2 RX Interrupts
 */

#include 	<stdio.h>
#include 	"uart.h"

#define GPIOAEN       (1U<<17)
#define PIN5          (1U<<5)
#define LED_PIN       PIN5


#define UART_BAUDRATE	115200

volatile char key;

static void uart_callback(void);
volatile uint8_t rx_flag;

int main (void){

    /* Initialize USART2 at 115200 baud */
	uart2_rx_interrupt_init(UART_BAUDRATE);

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER &= ~(3U << 10);   // clear both bits
	GPIOA->MODER |= (1U<<10);

	while(1){
	    if (rx_flag)
	    {
	        rx_flag = 0;
	        uart_callback();
	    }
	}
}
static void uart_callback(void){

	if(key == '1'){
		GPIOA->ODR |= LED_PIN;
	}else{
		GPIOA->ODR &= ~LED_PIN;
	}
}
void USART2_IRQHandler(void){
	if(USART2->ISR & ISR_RXNE){		//check if RXNE is set
		key = USART2->RDR;
		//Do something
		 rx_flag = 1;
	}
}
