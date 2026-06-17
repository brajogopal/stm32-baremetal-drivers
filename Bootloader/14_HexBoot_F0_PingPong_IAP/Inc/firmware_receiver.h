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
#include <stdbool.h>


#define FW_HEADER 0xAA

typedef enum
{
    WAIT_HEADER,
    READ_LENGTH,
    READ_CRC,
    RECEIVE_PAYLOAD,
    FW_COMPLETE,
    FW_ERROR
} fw_rx_state_t;




void firmware_rx_init(void);
void firmware_rx_process_byte(uint8_t data);

fw_rx_state_t firmware_rx_get_state(void);

bool firmware_chunk_ready(void);

uint8_t *firmware_get_flash_buffer(void);

uint16_t firmware_get_chunk_length(void);

#endif /* FIRMWARE_RECEIVER_H_ */
