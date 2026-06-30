/*
 * firmware_pingpong.h
 *
 * Public interface for Ping-Pong buffer management.
 *
 * Responsibilities:
 * - Buffer initialization.
 * - Buffer swapping.
 * - Buffer ownership management.
 *
 * Ping-Pong buffering allows firmware reception and Flash programming
 * to execute independently.
 *
 * ==================================================================
 *  Created on: 18-Jun-2026
 *      Author: brajo
 */

#ifndef FIRMWARE_PINGPONG_H_
#define FIRMWARE_PINGPONG_H_

#include <stdint.h>

#define FW_BUFFER_SIZE 128


typedef struct
{
    uint8_t bufferA[FW_BUFFER_SIZE];
    uint8_t bufferB[FW_BUFFER_SIZE];

    uint8_t *rx_buffer;
    uint8_t *prog_buffer;

    uint16_t rx_index;

    uint8_t rx_ready;
    uint8_t prog_ready;

} FW_PingPong_t;


extern FW_PingPong_t fw_pingpong;
extern volatile uint16_t len;


void firmware_pingpong_init(FW_PingPong_t *fw);
void swap_buffers(FW_PingPong_t *fw);
void firmware_pingpong_process_byte(FW_PingPong_t *fw, uint8_t data);

#endif /* FIRMWARE_PINGPONG_H_ */
