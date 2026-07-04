/*
 * firmware_pingpong.c
 *
 * Project : HexBoot_F0_DMA_PingPong_IAP
 *
 * Description:
 * Implements Ping-Pong buffering used during firmware updates.
 *
 * Double buffering allows DMA reception and Flash programming to
 * operate independently. While one buffer is being programmed into
 * Flash, DMA continues receiving the next firmware chunk into the
 * alternate buffer.
 *
 * Responsibilities:
 * - Initialize buffers.
 * - Swap active buffers.
 * - Maintain buffer ownership.
 */

#include "firmware_pingpong.h"

FW_PingPong_t fw_pingpong;
volatile uint16_t len =0;



/******************************************************************************
 * Public Functions
 ******************************************************************************/
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
