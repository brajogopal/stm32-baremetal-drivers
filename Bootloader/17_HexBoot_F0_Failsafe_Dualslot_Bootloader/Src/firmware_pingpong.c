/*
 * firmware_pingpong.c
 *
 * Ping-Pong buffer management.
 */

#include "firmware_pingpong.h"


/* Global Ping-Pong buffer */
FW_PingPong_t fw_pingpong;


/******************************************************************************
 * Public Functions
 ******************************************************************************/


void firmware_pingpong_init(FW_PingPong_t *fw)
{
	fw->rx_buffer = fw->bufferA;
	fw->prog_buffer = fw->bufferB;

	fw->prog_ready = 0;
}


void swap_buffers(FW_PingPong_t *fw)
{
	uint8_t *temp;

	temp = fw->rx_buffer;
	fw->rx_buffer = fw->prog_buffer;
	fw->prog_buffer = temp;

	fw->prog_ready = 1;
}
