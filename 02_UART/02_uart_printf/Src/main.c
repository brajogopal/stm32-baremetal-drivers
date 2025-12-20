#include "stm32f030x8.h"
#include <stdint.h>
#include <stdio.h>

#define GPIOAEN     	(1U<<17)
#define USART2EN		(1U<<17)
#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<0)
#define ISR_TXE			(1U<<7)

#define SYS_FREQ 		8000000
#define APB1_CLK 		SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void usart2_tx_init();
void uart2_write(int ch);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

int main (void){

	usart2_tx_init();
	while(1){
		printf("hello from stm32..\n\r");
	}
}

void usart2_tx_init(){
	/**********configure usart gpio pin*************/
	/*Enable clock access to GPIOA*/
	RCC->AHBENR |= GPIOAEN;
	/*set PA2 mode to Alternate func*/
	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~ (1U<<4);
	/*PA2 high speed*/
	GPIOA->OSPEEDR |= (1U << 5);
	/*set PA2 alternate func type to usart_tx(AF1)*/
	GPIOA->AFR[0] |= (1U<<8);		//AFR[0] for AFRL
	GPIOA->AFR[0] &=~ (1U<<9);
	GPIOA->AFR[0] &=~ (1U<<10);
	GPIOA->AFR[0] &=~ (1U<<11);

	/***********Configure Usart module***********/
	/*Enable clock access to USART2*/
	RCC->APB1ENR |= USART2EN;
	/*Configure Baudrate*/
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	/*configure the transfer direction*/
	USART2->CR1 |= CR1_TE;
	/*Enable usart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch){
	/*Make sure that the transmit data register is empty*/
	while(!(USART2->ISR & ISR_TXE));
	/*write to transmit data register*/
	USART2->TDR = (ch & 0xFF);

}
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return((PeriphClk+(BaudRate/2))/BaudRate);
}
