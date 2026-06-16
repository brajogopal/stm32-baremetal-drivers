/*
 * firmware_receiver.h
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#ifndef FIRMWARE_RECEIVER_H_
#define FIRMWARE_RECEIVER_H_

#include <stdint.h>
#include "uart.h"
#include "flash_driver.h"


typedef enum
{
    WAIT_HEADER,
    READ_LENGTH,
    READ_CRC,
    RECEIVE_PAYLOAD,
    FW_COMPLETE,
    FW_ERROR
} fw_rx_state_t;


flash_status_t erase_flash_region(uint32_t start_address, uint32_t size_bytes);
flash_status_t program_flash_chunk(uint32_t flash_address, uint16_t *chunk_buffer, uint32_t halfword_count);

void firmware_rx_init(void);
void firmware_rx_process_byte(uint8_t data);

fw_rx_state_t firmware_rx_get_state(void);

#endif /* FIRMWARE_RECEIVER_H_ */
