/*
 * uart.c
 *
 *  Created on: Dec 19, 2025
 *      Author: brajo
 */
#include 	"uart.h"
#include "stm32f030x8.h"

/* ---------- Private macros ---------- */
#define GPIOA_EN     	(1U<<17)
#define USART2_EN		(1U<<17)

#define CR1_RE			(1U<<2)
#define CR1_TE			(1U<<3)

#define CR1_UE			(1U<<0)
#define ISR_TXE			(1U<<7)
#define ISR_RXNE		(1U<<5)

#define AHBENR_DMAEN	(1U<<0)
#define DMA_CCR_EN		(1U<<0)
#define DMA_CCR_MINC	(1U<<7)
#define DMA_CCR_DIR		(1U<<4)
#define DMA_CCR_TEIE	(1U<<3)
#define USART2_CR3_DMAT	(1U<<7)



/* ---------- Public functions ---------- */
static uint16_t compute_uart_brr(uint32_t pclk, uint32_t baudrate);
void uart2_tx_init(uint32_t baudrate);
void uart2_rxtx_init(uint32_t baudrate);
char uart2_read(void);
void uart2_write(char ch);
int __io_putchar(int ch);		/* Redirect printf() to USART2 */




/* Compute USART BRR value (oversampling by 16) */
static uint16_t compute_uart_brr(uint32_t pclk, uint32_t baudrate)
{
    return (pclk + (baudrate / 2U)) / baudrate;
}

/* Get APB1 clock frequency (USART2 clock) */
static uint32_t get_pclk1_freq(void)
{
    uint32_t sysclk;
    uint32_t clk_src = (RCC->CFGR >> 2) & 0x3;
    uint32_t hpre    = (RCC->CFGR >> 4) & 0xF;
    uint32_t ppre    = (RCC->CFGR >> 8) & 0x7;
    /* Determine system clock source */
    switch (clk_src)
    {
        case 0x0:  // HSI
            sysclk = 8000000U;
            break;

        case 0x1:  // HSE
            sysclk = 8000000U;   // adjust if crystal differs
            break;

        case 0x2:  // PLL
            sysclk = 48000000U;
            break;

        default:   // reserved / unexpected
            sysclk = 8000000U;   // safe fallback
            break;
    }
    /* Apply AHB prescaler */
    if (hpre >= 8) {
        sysclk >>= (hpre - 7);
    }
    /* Apply APB1 prescaler */
    if (ppre >= 4) {
        sysclk >>= (ppre - 3);
    }
    return sysclk;
}

/* Initialize USART2 TX on PA2 (AF1) */
void uart2_tx_init(uint32_t baudrate)
{
    RCC->AHBENR |= GPIOA_EN; 	  /* Enable GPIOA clock */

    /* PA2 → Alternate function mode */
    GPIOA->MODER &= ~(3U << 4);    //clear bit 4 & 5
    GPIOA->MODER |=  (2U << 4);    //set MODER2 as 10 for AF

    GPIOA->OSPEEDR |= (1U << 5);    /* PA2 → High speed */

    /* PA2 → AF1 (USART2_TX) */
    GPIOA->AFR[0] &= ~(0xFU << 8);  //Clearing all the bits from 8 to 11
    GPIOA->AFR[0] |=  (1U << 8); 	//seting PA2 AF mode as 0001

    RCC->APB1ENR |= USART2_EN;    /* Enable USART2 clock */

    USART2->BRR = compute_uart_brr(get_pclk1_freq(), baudrate);    /* Set baud rate */

    USART2->CR1 |= CR1_TE ;    /* Enable transmitter */

    USART2->CR1 |= CR1_UE;    /* Enable USART */
}

/* Initialize USART2 RX & TX on PA2 and PA3 (AF1) */
void uart2_rxtx_init(uint32_t baudrate)
{
    RCC->AHBENR |= GPIOA_EN; 	  /* Enable GPIOA clock */

    /* PA2 → Alternate function mode */
    GPIOA->MODER &= ~(3U << 4);    //clear bit 4 & 5
    GPIOA->MODER |=  (2U << 4);    //set MODER2 as 10 for AF
    GPIOA->OSPEEDR |= (1U << 5);    /* PA2 → High speed */

    /* PA2 → AF1 (USART2_TX) */
    GPIOA->AFR[0] &= ~(0xFU << 8);  //Clearing all the bits from 8 to 11
    GPIOA->AFR[0] |=  (1U << 8); 	//seting PA2 AF mode as 0001

    /* PA3 → Alternate function mode */
    GPIOA->MODER &= ~(3U << 6);			//clear bit 6 & 7
    GPIOA->MODER |=  (2U << 6);			//set MODER3 as 10 for AF
    GPIOA->OSPEEDR |= (1U << 7);   		/* PA3 → High speed */
    /* PA3 → AF1 (USART2_RX) */
    GPIOA->AFR[0] &= ~(0xFU << 12);  //Clearing all the bits from 8 to 11
    GPIOA->AFR[0] |=  (1U << 12); 	//seting PA3 AF mode as 0001

    RCC->APB1ENR |= USART2_EN;    /* Enable USART2 clock */

    USART2->BRR = compute_uart_brr(get_pclk1_freq(), baudrate);    /* Set baud rate */

    USART2->CR1 |= (CR1_TE | CR1_RE);    /* Enable transmitter */

    USART2->CR1 |= CR1_UE;    /* Enable USART */
}

/* Initialize USART2 RX interrupt on and PA3 (AF1) */
void uart2_rx_interrupt_init(uint32_t baudrate)
{
    RCC->AHBENR |= GPIOA_EN; 	  /* Enable GPIOA clock */


    /* PA2 → Alternate function mode */
    GPIOA->MODER &= ~(3U << 4);    //clear bit 4 & 5
    GPIOA->MODER |=  (2U << 4);    //set MODER2 as 10 for AF
    GPIOA->OSPEEDR &= ~(3U << 4);
    GPIOA->OSPEEDR |=  (2U << 4);   // PA2 high speed

    /* PA2 → AF1 (USART2_TX) */
    GPIOA->AFR[0] &= ~(0xFU << 8);  //Clearing all the bits from 8 to 11
    GPIOA->AFR[0] |=  (1U << 8); 	//seting PA2 AF mode as 0001



    /* PA3 → Alternate function mode */
    GPIOA->MODER &= ~(3U << 6);			//clear bit 6 & 7
    GPIOA->MODER |=  (2U << 6);			//set MODER3 as 10 for AF
    GPIOA->OSPEEDR &= ~(3U << 6);
    GPIOA->OSPEEDR |=  (2U << 6);	   // PA3 high speed

    /* PA3 → AF1 (USART2_RX) */
    GPIOA->AFR[0] &= ~(0xFU << 12);  //Clearing all the bits from 8 to 11
    GPIOA->AFR[0] |=  (1U << 12); 	//seting PA3 AF mode as 0001



    RCC->APB1ENR |= USART2_EN;    /* Enable USART2 clock */

    USART2->BRR = compute_uart_brr(get_pclk1_freq(), baudrate);    /* Set baud rate */

    USART2->CR1 |= (CR1_TE | CR1_RE);    /* Enable transmitter */

    USART2->CR1 |= USART_CR1_RXNEIE;		/*RX Interrupt*/

    __NVIC_EnableIRQ(USART2_IRQn);		/*Enable UART2 Interrupt in NVIC*/

    USART2->CR1 |= CR1_UE;	    /* Enable USART */

}

void dma_uart2_tx_init(uint32_t src,uint32_t dst,uint32_t len){
	//1.Enable clock access to DMA
	RCC->AHBENR |= AHBENR_DMAEN;

	//2.Disable DMA channel before configuration
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;

	//wait until dma1_channel is disabled
	while(DMA1_Channel4->CCR & DMA_CCR_EN){}

	//3.Clear interrupt flag for ch4
	DMA1->IFCR |= (0xF<<12);

	//4.set peripheral address (Usart2_tdr)
	DMA1_Channel4->CPAR = dst;

	//5.set memory address (TX buffer)
	DMA1_Channel4->CMAR = src;

	//6.set Length
	DMA1_Channel4->CNDTR = len;

	//7.Enable memory increment
	DMA1_Channel4->CCR |= DMA_CCR_MINC;

	//8.configure transfer direction
	DMA1_Channel4->CCR |= DMA_CCR_DIR;

	//9.transfer complete interrupt
	DMA1_Channel4->CCR |= DMA_CCR_TEIE;

	//11.Enable DMA channel
	DMA1_Channel4->CCR |= DMA_CCR_EN;

	//12.Enable uart2 transmitter in DMA
	USART2->CR3 |= USART2_CR3_DMAT;

	//13.Dma interrupt enable in nvic
	__NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}


char uart2_read(void){
	/*make sure the receive data register isn't empty*/
    while (!(USART2->ISR & ISR_RXNE)){};	/* Wait until RXNE is empty */
    return USART2->RDR; 					//READ DATA
}

/* Transmit one character */
void uart2_write(char ch)
{
    /* Wait until transmit buffer is empty */
    while (!(USART2->ISR & ISR_TXE));
    USART2->TDR = ch;
}

/* Redirect printf() to USART2 */
int __io_putchar(int ch)
{
    uart2_write((char)ch);
    return ch;
}
