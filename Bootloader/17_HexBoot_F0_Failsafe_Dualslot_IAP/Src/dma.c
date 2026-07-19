/*
 * dma.c
 *
 * Project : HexBoot_F0_DMA_PingPong_IAP
 *
 * Description:
 * Implements the DMA driver used by the bootloader to receive firmware
 * over UART. The DMA peripheral transfers incoming UART data directly
 * into memory, reducing CPU overhead during firmware updates.
 *
 * Responsibilities:
 * - Initialize DMA Channel 5 for UART reception.
 * - Configure DMA transfer destination and transfer size.
 * - Start a new DMA reception.
 *
 * Notes:
 * - The DMA driver is transport-only.
 * - It does not understand firmware packets or bootloader logic.
 * - Buffer ownership is managed by firmware_receiver.c.
 */

#include "dma.h"
#include "firmware_receiver.h"




/******************************************************************************
 * Public Functions
 ******************************************************************************/
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



/********************************************************************
 *  Configure DMA reception
 *
 * This function is called by the firmware reception state machine
 * whenever a new firmware transfer is required.
********************************************************************/
void dma_receive(void *buffer, uint16_t size){
	__disable_irq();

	DMA1_Channel5->CCR &= ~DMA_CCR_EN;

	DMA1_Channel5->CMAR = (uint32_t)buffer;
	DMA1_Channel5->CNDTR = size;

	DMA1_Channel5->CCR |= DMA_CCR_EN;

	__enable_irq();
}




void DMA1_CH4_5_IRQHandler(void){
    if(DMA1->ISR & DMA_ISR_TCIF5)	//Read
    {
    	println("DMA COMPLETE");
        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
        firmware_receiver_dma_callback(1);
    }

    if(DMA1->ISR & DMA_ISR_TEIF5)	//Read
    {
        println("DMA ERROR");

        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
    }
}
