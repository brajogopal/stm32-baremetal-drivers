#include "stm32f030x8.h"
#include <stdint.h>

#define GPIOAEN     	(1U<<17)
#define USART2EN		(1U<<17)
#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<0)
#define ISR_TXE			(1U<<7)



#define HSI_FREQ   8000000U
#define HSE_FREQ   8000000U      // change if external crystal differs
#define PLL_FREQ  48000000U


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
	  uint32_t sysclk;
	  uint32_t cfgr = RCC->CFGR;

	    /* -------- Determine system clock source -------- */
	    switch ((cfgr >> 2) & 0x3)
	    {
	        case 0x0:  // HSI
	            sysclk = HSI_FREQ;
	            break;

	        case 0x1:  // HSE
	            sysclk = HSE_FREQ;
	            break;

	        case 0x2:  // PLL
	            sysclk = PLL_FREQ;
	            break;

	        default:   // fallback (should not happen)
	            sysclk = HSI_FREQ;
	            break;
	    }

    /* -------- Apply AHB prescaler -------- */
    /*
     * Bits [7:4] of RCC_CFGR = HPRE (AHB prescaler)
     *
     * HPRE encoding (from reference manual):
     * 0xxx : SYSCLK not divided
     * 1000 : SYSCLK / 2
     * 1001 : SYSCLK / 4
     * 1010 : SYSCLK / 8
     * 1011 : SYSCLK / 16
     * 1100 : SYSCLK / 64
     * 1101 : SYSCLK / 128
     * 1110 : SYSCLK / 256
     * 1111 : SYSCLK / 512
     *
     * For values 0–7, there is NO division.
     * For values 8–15, division factor = 2^(HPRE - 7)
     */

    uint32_t hpre = (cfgr >> 4) & 0xF;   // extract HPRE bits

    if (hpre >= 8)
    {
        /* Example:
         * HPRE = 8  -> divide by 2  (2^(8 - 7))
         * HPRE = 9  -> divide by 4  (2^(9 - 7))
         * HPRE = 10 -> divide by 8  (2^(10 - 7))
         */
        sysclk >>= (hpre - 7);   // apply AHB prescaler
    }

    /* -------- Apply APB1 prescaler -------- */
    /*
     * Bits [10:8] of RCC_CFGR = PPRE (APB1 prescaler)
     *
     * PPRE encoding:
     * 0xx : HCLK not divided
     * 100 : HCLK / 2
     * 101 : HCLK / 4
     * 110 : HCLK / 8
     * 111 : HCLK / 16
     *
     * For values 0–3, there is NO division.
     * For values 4–7, division factor = 2^(PPRE - 3)
     */

    uint32_t ppre = (cfgr >> 8) & 0x7;   // extract PPRE bits

    if (ppre >= 4)
    {
        /* Example:
         * PPRE = 4 -> divide by 2  (2^(4 - 3))
         * PPRE = 5 -> divide by 4  (2^(5 - 3))
         * PPRE = 6 -> divide by 8  (2^(6 - 3))
         */
        sysclk >>= (ppre - 3);   // apply APB1 prescaler
    }


    return sysclk;
}
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return((PeriphClk+(BaudRate/2))/BaudRate);
}
