/*
 * firmware_pingpong.h
 *
 * Public interface for Ping-Pong buffer management.
 */

#ifndef FIRMWARE_PINGPONG_H_
#define FIRMWARE_PINGPONG_H_

#include <stdint.h>

#define FW_BUFFER_SIZE (128U)


typedef struct
{
    uint8_t bufferA[FW_BUFFER_SIZE];
    uint8_t bufferB[FW_BUFFER_SIZE];

    uint8_t *rx_buffer;
    uint8_t *prog_buffer;

    uint8_t prog_ready;

} FW_PingPong_t;


extern FW_PingPong_t fw_pingpong;

void firmware_pingpong_init(FW_PingPong_t *fw);
void swap_buffers(FW_PingPong_t *fw);

#endif /* FIRMWARE_PINGPONG_H_ */
