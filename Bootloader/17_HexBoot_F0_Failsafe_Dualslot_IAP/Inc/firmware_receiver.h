/*
 * firmware_receiver.h
 *
 * Public interface for the firmware reception state machine.
 *
 * Responsibilities:
 * - Firmware reception.
 * - Firmware state definitions.
 * - Header parsing.
 * - Firmware progress tracking.
 *
 * This module coordinates DMA reception and firmware processing.
 *
 * ==============================================================
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#ifndef FIRMWARE_RECEIVER_H_
#define FIRMWARE_RECEIVER_H_

#include <flash.h>
#include <stdint.h>
#include "uart.h"

#define CHUNK_SIZE 128

typedef struct
{
    uint8_t  header;
    uint16_t payload_length;
    uint16_t crc;
} FW_Header_t;



/******************************************************
 * Firmware Reception State Machine
 *
 * RECEIVE_HEADER
 *     Configure DMA to receive the packet header.
 *
 * PARSE_HEADER
 *     Validate the received header and extract metadata.
 *
 * START_PAYLOAD_DMA
 *     Configure DMA to receive the first firmware chunk.
 *
 * RECEIVE_PAYLOAD
 *     Process a completed firmware chunk.
 *
 * FW_COMPLETE
 *     Firmware reception completed successfully.
 *
 * FW_ERROR
 *     Reserved for future error handling.
 ****************************************************/
typedef enum
{
    RECEIVE_HEADER,
    PARSE_HEADER,
	START_PAYLOAD_DMA,
    RECEIVE_PAYLOAD,
    FW_COMPLETE,
    FW_ERROR
} fw_rx_state_t;



extern uint16_t payload_length;
extern volatile uint32_t header_received;
extern volatile uint32_t bytes_received;
extern uint16_t expected_crc;
extern volatile uint8_t program_chunk_size;



fw_rx_state_t firmware_rx_get_state(void);

void firmware_rx_process(void);

uint16_t firmware_get_next_chunk_size(void);



#endif /* FIRMWARE_RECEIVER_H_ */
