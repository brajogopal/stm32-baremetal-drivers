/*
 * firmware_receiver.h
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#ifndef FIRMWARE_RECEIVER_H_
#define FIRMWARE_RECEIVER_H_

#include "uart.h"
#include "flash_driver.h"

uint16_t reconstruct_halfword(void);
uart_status_t reconstruct_halfword_timeout(uint16_t *data,uint32_t timeout);
uart_status_t firmware_receive_chunk(uint32_t addr,uint16_t *data, uint32_t halfwords, uint32_t timeout);
uart_status_t firmware_receive_8bit(uint32_t addr,uint8_t *data, uint32_t length, uint32_t timeout);
flash_status_t store_data_in_flash(uint32_t addr,uint16_t *data, uint32_t length);

flash_status_t erase_flash_region(uint32_t start_address, uint32_t size_bytes);

flash_status_t program_flash_chunk(uint32_t flash_address, uint16_t *chunk_buffer, uint32_t halfword_count);


#endif /* FIRMWARE_RECEIVER_H_ */
