/*
 * dma.c
 *
 *  Created on: 24-Jun-2026
 *      Author: brajo
 */

#include "dma.h"
#include "firmware_receiver.h"


void dma_init(void){

RCC->AHBENR |= RCC_DMA_EN;
USART2->CR3 |= USART_DMA_RE;
/*
 * To configure DMA Channel
 * DMA1_CSELR &= ~(0xFUL << 16U);
 * DMA1_CSELR |=  (0x9UL << 16U); // [1001] - To dedicate channel 5 to USART2_ RX
*/
DMA1_Channel5->CPAR = (uint32_t)(&(USART2->RDR));
DMA1_Channel5->CCR |= DMA_CCR_MINC | DMA_CCR_TEIE | DMA_CCR_TCIE ;

/* Configure NVIC for DMA */
/* (1) Enable Interrupt on DMA Channel 5 */
/* (2) Set priority for DMA Channel 5 */
NVIC_EnableIRQ(DMA1_Channel4_5_IRQn); /* (1) */
NVIC_SetPriority(DMA1_Channel4_5_IRQn,0); /* (2) */
}




void dma_receive(void *buffer, uint16_t size){
	__disable_irq();

	DMA1_Channel5->CCR &= ~DMA_CCR_EN;

	DMA1_Channel5->CMAR = (uint32_t)buffer;
	DMA1_Channel5->CNDTR = size;

	DMA1_Channel5->CCR |= DMA_CCR_EN;

	__enable_irq();
}
