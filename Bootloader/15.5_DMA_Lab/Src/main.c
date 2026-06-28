#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"
#include "crc.h"
#include <stdio.h>


#define DMA1_CSELR_ADDR (DMA1_BASE + 0xA8UL)
#define DMA1_CSELR      (*(volatile uint32_t *)DMA1_CSELR_ADDR)

#define RCC_DMA_EN		(1UL << 0U)
#define USART_DMA_RE	(1UL << 6U)
#define DMA_EN			(1UL << 0U)
volatile uint8_t dma_re = 0;

int main(){

firmware_pingpong_init(&fw_pingpong);
debug_uart_init(38400);

/* The following example is given for the ADC. It can be easily ported on
any peripheral supporting DMA transfer taking of the associated channel
to the peripheral, this must check in the datasheet. */
/* (1) Enable the peripheral clock on DMA */
/* (2) Enable DMA transfer on USART2  */
/* (3) Configure the peripheral data register address */
/* (4) Configure the memory address */
/* (5) Configure the number of DMA transfer to be performs on channel 5 */
/* (6) Configure increment, size and interrupts */
/* (7) Enable DMA Channel 5 */

RCC->AHBENR |= RCC_DMA_EN; /* (1) */
USART2->CR3 |= USART_DMA_RE;/* (2) */
/*
 * To configure DMA Channel
 * DMA1_CSELR &= ~(0xFUL << 16U);
 * DMA1_CSELR |=  (0x9UL << 16U); // [1001] - To dedicate channel 5 to USART2_ RX
*/
DMA1_Channel5->CPAR = (uint32_t) (&(USART2->RDR)); /* (3) */
DMA1_Channel5->CMAR = (uint32_t)(fw_pingpong.rx_buffer); /* (4) */
DMA1_Channel5->CNDTR = FW_BUFFER_SIZE; /* (5) */
DMA1_Channel5->CCR |= DMA_CCR_MINC | DMA_CCR_TEIE | DMA_CCR_TCIE ; /* (6) */
DMA1_Channel5->CCR |= DMA_EN; /* (7) */

/* Configure NVIC for DMA */
/* (1) Enable Interrupt on DMA Channel 5 */
/* (2) Set priority for DMA Channel 5 */
NVIC_EnableIRQ(DMA1_Channel4_5_IRQn); /* (1) */
NVIC_SetPriority(DMA1_Channel4_5_IRQn,0); /* (2) */


while(1){
	if(dma_re){
		for(uint32_t i=0; i<FW_BUFFER_SIZE; i++)
		{
		    printf("%c", fw_pingpong.rx_buffer[i]);
		}
		printf("\r\n");
		dma_re = 0;
	}
}
}


void DMA1_CH4_5_IRQHandler(void){
    if(DMA1->ISR & DMA_ISR_TCIF5)	//Read
    {
        println("DMA COMPLETE");
        dma_re = 1;
        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
    }

    if(DMA1->ISR & DMA_ISR_TEIF5)	//Read
    {
        println("DMA ERROR");

        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
    }
}
