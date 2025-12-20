#include "stm32f030x8.h"
#include <stdint.h>

#define GPIOAEN     	(1U<<17)
#define USART2EN		(1U<<17)
#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<0)
#define ISR_TXE			(1U<<7)

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void usart2_tx_init();
void uart2_write(int ch);
uint32_t get_pclk1_freq(void);

int main (void){

	usart2_tx_init();
	while(1){
		uart2_write('y');
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
	uart_set_baudrate(USART2, get_pclk1_freq(), UART_BAUDRATE);
	/*configure the transfer direction*/
	USART2->CR1 = CR1_TE;
	/*Enable usart module*/
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch){
	/*Make sure that the transmit data register is empty*/
	while(!(USART2->ISR & ISR_TXE));
	/*write to transmit data register*/
	USART2->TDR = (ch & 0xFF);
}
uint32_t get_pclk1_freq(void)
{
    uint32_t sysclk = 8000000; // default HSI
    uint32_t clk_src = (RCC->CFGR >> 2) & 0x3;

    if (clk_src == 0x0) {
        sysclk = 8000000;   // HSI
    } else if (clk_src == 0x1) {
        sysclk = 8000000;   // HSE (change if crystal differs)
    } else if (clk_src == 0x2) {
        sysclk = 48000000;  // PLL (example)
    }

    uint32_t hpre = (RCC->CFGR >> 4) & 0xF;
    uint32_t ppre = (RCC->CFGR >> 8) & 0x7;

    if (hpre >= 8) sysclk >>= (hpre - 7);
    if (ppre >= 4) sysclk >>= (ppre - 3);

    return sysclk;
}
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return((PeriphClk+(BaudRate/2))/BaudRate);
}
