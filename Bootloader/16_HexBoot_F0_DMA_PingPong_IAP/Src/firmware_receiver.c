/*
 * firmware_receiver.c
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#include "uart.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"
#include "dma.h"
#include <stdio.h>

#define FW_HEADER 0xAA

static volatile fw_rx_state_t rx_state = RECEIVE_HEADER;


uint16_t payload_length = 0;
uint16_t expected_crc = 0;

volatile uint8_t flash_chunk_size = 0;
volatile uint8_t erase_whole = 0;
volatile uint8_t last_chunk = 0;
volatile uint32_t header_received = 0;
volatile uint32_t bytes_received = 0;
volatile uint8_t chunk_ready = 0;
volatile uint8_t rx_buffer[CHUNK_SIZE];
uint16_t current_chunk_size = 0;

static FW_Header_t fw_header;

/*
 * Project 16:
 * Header is received using DMA into FW_Header_t.
 * Byte-wise states READ_LENGTH and READ_CRC
 * are no longer required.
 *
 *
 * * Event-driven state machine.
 *
 * Each call performs one state action and
 * prepares the next state.
 *
 * The state machine is invoked after DMA
 * transfer completion events.
 */
void firmware_rx_process(void)
{
    switch(rx_state)
    {
        case RECEIVE_HEADER:
        	dma_receive(&fw_header, sizeof(FW_Header_t));
        	rx_state = PARSE_HEADER;
            break;


        case PARSE_HEADER:
        	if(fw_header.header == FW_HEADER){
        		payload_length = fw_header.payload_length;
        		expected_crc = fw_header.crc;
        		header_received = 1;
        		rx_state = START_PAYLOAD_DMA;
        	}	else
        	{
        		println("wrong header file");
        		while(1);
        	}
            break;


        case START_PAYLOAD_DMA:
        	current_chunk_size = firmware_get_next_chunk_size();
        	dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
        	rx_state =  RECEIVE_PAYLOAD;
        	break;


        case RECEIVE_PAYLOAD:
        	flash_chunk_size = current_chunk_size;
        	swap_buffers(&fw_pingpong);
        	bytes_received += current_chunk_size;
        	fw_pingpong.prog_ready = 1;
        	current_chunk_size = firmware_get_next_chunk_size();


            if(current_chunk_size > 0)
            {
                dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
            }
            else
            {
                rx_state = FW_COMPLETE;
                println("firmware receive complete");
            }

        	break;


        case FW_COMPLETE:
        	/* Reserved for future use */

            break;

        case FW_ERROR:
        	/* Reserved for future error handling */

            break;
    }
}


fw_rx_state_t firmware_rx_get_state(void)
{
    return rx_state;
}

uint16_t firmware_get_next_chunk_size(void)
{
    uint32_t remaining;

    remaining = payload_length - bytes_received;

    if(remaining > FW_BUFFER_SIZE)
    {
        return FW_BUFFER_SIZE;
    }

    return remaining;
}




