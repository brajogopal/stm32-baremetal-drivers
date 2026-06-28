/*
 * dma.h
 *
 *  Created on: 24-Jun-2026
 *      Author: brajo
 */
#include <stdint.h>

#ifndef DMA_H_
#define DMA_H_

#define DMA1_CSELR_ADDR (DMA1_BASE + 0xA8UL)
#define DMA1_CSELR      (*(volatile uint32_t *)DMA1_CSELR_ADDR)

#define RCC_DMA_EN		(1UL << 0U)
#define USART_DMA_RE	(1UL << 6U)
#define DMA_EN			(1UL << 0U)


void dma_init(void);
void dma_receive(void *buffer, uint16_t size);

#endif /* DMA_H_ */
