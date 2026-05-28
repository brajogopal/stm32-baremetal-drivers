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
flash_status_t store_data_in_flash(uint32_t addr,uint16_t *data, uint32_t length);
flash_status_t firmware_receive_chunk(uint32_t addr,uint16_t *data, uint32_t length, uint32_t timeout);

#endif /* FIRMWARE_RECEIVER_H_ */
