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



#define CHUNK_SIZE 128





typedef struct
{
    uint8_t  header;
    uint16_t payload_length;
    uint16_t crc;
} FW_Header_t;

/*
 * Firmware Reception State Machine
 */
typedef enum
{
    RECEIVE_HEADER,
    PARSE_HEADER,
	START_PAYLOAD_DMA,
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
extern volatile uint32_t header_received;
extern volatile uint32_t bytes_received;
extern uint16_t expected_crc;
extern volatile uint8_t chunk_ready;
extern volatile uint8_t rx_buffer[CHUNK_SIZE];
extern volatile uint8_t last_chunk;
extern volatile uint8_t flash_chunk_size;


void firmware_rx_init(void);
void firmware_rx_process_byte(uint8_t data);

fw_rx_state_t firmware_rx_get_state(void);

bool firmware_chunk_ready(void);

uint8_t *firmware_get_flash_buffer(void);

uint16_t firmware_get_chunk_length(void);

void firmware_rx_process(void);

uint16_t firmware_get_next_chunk_size(void);



#endif /* FIRMWARE_RECEIVER_H_ */
