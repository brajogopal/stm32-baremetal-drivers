/*
 * dma.h
 *
 * This module provides the public interface for the DMA driver.
 *
 * Responsibilities:
 * - DMA initialization.
 * - DMA transfer configuration.
 * - DMA receive API.
 *
 * This module does NOT:
 * - Parse firmware packets.
 * - Program Flash.
 * - Perform CRC verification.
 *
 * =============================================================
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


/*
 * Configure and start a DMA reception.
 *
 * Parameters:
 *  buffer - Destination memory buffer.
 *  size   - Number of bytes to receive.
 *
 * Returns:
 *  None
 *
 * Notes:
 *  Called by the firmware reception state machine whenever
 *  a new DMA transfer is required.
 */
void dma_receive(void *buffer, uint16_t size);

#endif /* DMA_H_ */
