/*
 * firmware_receiver.c
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#include "uart.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"
#include <stdio.h>



static volatile fw_rx_state_t rx_state = WAIT_HEADER;

static uint8_t parser_index = 0;

static uint8_t length_bytes[2];
static uint8_t crc_bytes[2];

uint16_t payload_length = 0;
uint16_t expected_crc = 0;

volatile uint8_t erase_whole = 0;
volatile uint8_t last_chunk = 0;
volatile uint32_t bytes_received = 0;
volatile uint8_t chunk_ready = 0;
volatile uint8_t rx_buffer[CHUNK_SIZE];



void firmware_rx_process_byte(uint8_t data)
{
    switch(rx_state)
    {
        case WAIT_HEADER:
        	if (data == FW_HEADER){
        		println("Header matched");
        	    parser_index = 0;
        		rx_state = READ_LENGTH;
        	}
            break;


        case READ_LENGTH:
        	length_bytes[parser_index++] = data;
        	if(parser_index == 2){
        		payload_length = (length_bytes[0] | ((uint16_t)length_bytes[1] << 8U));
        		parser_index = 0;
        		printf("Length: %u\r\n", payload_length);
        		erase_whole = 1;
        		rx_state = READ_CRC;
        	}

        	break;


        case READ_CRC:
        	crc_bytes[parser_index++] = data;
        	if(parser_index == 2){
        		expected_crc = (crc_bytes[0] | ((uint16_t)crc_bytes[1] << 8U));
        		parser_index = 0;
        		printf("CRC: 0x%04X\r\n", expected_crc);
        	    rx_state = RECEIVE_PAYLOAD;
        	}
            break;


    	/*
    	 * Firmware payload is received in CHUNK_SIZE blocks.
   		 * When a chunk is full, chunk_ready is asserted and
   		 * main() programs the data into flash memory.
   		 */
        case RECEIVE_PAYLOAD:
        	firmware_pingpong_process_byte(&fw_pingpong,data);
        	bytes_received++;

        	if(bytes_received >= payload_length)
        	{
        		len = fw_pingpong.rx_index;
        		last_chunk = 1;
        	    swap_buffers(&fw_pingpong);
        	    fw_pingpong.rx_index = 0;
        	    rx_state = FW_COMPLETE;
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






