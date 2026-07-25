/*
 * firmware_receiver.h
 *
 * Public interface for the firmware reception state machine.
 */

#ifndef FIRMWARE_RECEIVER_H_
#define FIRMWARE_RECEIVER_H_

#include <flash.h>
#include <stdint.h>


#define CHUNK_SIZE 128

typedef struct
{
    uint8_t  header;
    uint16_t payload_length_A;
    uint16_t payload_length_B;
    uint16_t crc_A;
    uint16_t crc_B;
} FW_Header_t;


typedef enum
{
    PARSE_HEADER,
	START_PAYLOAD_DMA,
    RECEIVE_PAYLOAD,
    FW_COMPLETE,
    FW_ERROR
} fw_rx_state_t;


extern uint16_t payload_length_A;
extern uint16_t payload_length_B;
extern volatile uint32_t header_received;
extern volatile uint32_t bytes_received;
extern uint16_t expected_crc_A;
extern uint16_t expected_crc_B;
extern volatile uint8_t program_chunk_size;


void firmware_receiver_init(void);

void firmware_receiver_dma_callback(uint8_t value);

void firmware_set_payload_info(uint16_t length, uint16_t crc);

uint8_t get_rx_update(void);

fw_rx_state_t firmware_rx_get_state(void);

void firmware_rx_process(void);


#endif /* FIRMWARE_RECEIVER_H_ */
