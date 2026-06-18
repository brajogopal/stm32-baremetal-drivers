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
#define CHUNK_SIZE 128



/*
 * Firmware Reception State Machine
 *
 * WAIT_HEADER     : Wait for firmware start byte
 * READ_LENGTH     : Receive payload length (2 bytes)
 * READ_CRC        : Receive expected CRC16 (2 bytes)
 * RECEIVE_PAYLOAD : Receive firmware data
 * FW_COMPLETE     : Firmware reception completed
 * FW_ERROR        : Reserved for future error handling
 */
typedef enum
{
    WAIT_HEADER,
    READ_LENGTH,
    READ_CRC,
    RECEIVE_PAYLOAD,
    FW_COMPLETE,
    FW_ERROR
} fw_rx_state_t;



/*
 * Parser context
 * Maintains firmware reception state between UART interrupts.
 */
extern volatile uint8_t erase_whole;
extern uint16_t payload_length;
extern volatile uint32_t bytes_received;
extern uint16_t expected_crc;
extern volatile uint8_t chunk_ready;
extern volatile uint16_t len;
extern volatile uint8_t rx_buffer[CHUNK_SIZE];


void firmware_rx_init(void);
void firmware_rx_process_byte(uint8_t data);

fw_rx_state_t firmware_rx_get_state(void);

bool firmware_chunk_ready(void);

uint8_t *firmware_get_flash_buffer(void);

uint16_t firmware_get_chunk_length(void);

#endif /* FIRMWARE_RECEIVER_H_ */
