/*
 * firmware_pingpong.c
 *
 *  Created on: 18-Jun-2026
 *      Author: brajo
 */
#include "firmware_pingpong.h"

FW_PingPong_t fw_pingpong;
volatile uint16_t len =0;

void firmware_pingpong_init(FW_PingPong_t *fw){
	fw->rx_buffer = fw->bufferA;
	fw->prog_buffer = fw->bufferB;

	fw->rx_index = 0;
	fw->rx_ready = 0;
	fw->prog_ready = 0;
}



void swap_buffers(FW_PingPong_t *fw){
	uint8_t *temp;

	temp = fw->rx_buffer;
	fw->rx_buffer = fw->prog_buffer;
	fw->prog_buffer = temp;

	fw->prog_ready = 1;
}

void firmware_pingpong_process_byte( FW_PingPong_t *fw,uint8_t data){

	fw->rx_buffer[fw->rx_index++] = data;

	    if(fw->rx_index >= FW_BUFFER_SIZE)
	    {
	        swap_buffers(fw);
	        len = fw->rx_index;
	        fw->rx_index = 0;
	    }
}
