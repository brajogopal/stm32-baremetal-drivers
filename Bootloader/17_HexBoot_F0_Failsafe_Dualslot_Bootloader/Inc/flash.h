/*
 * flash.h
 *
 * Public interface for internal Flash memory operations.
 */

#ifndef FLASH_H_
#define FLASH_H_
#include <stdint.h>


typedef enum{
    FLASH_OK,
    FLASH_BUSY,
    FLASH_ERROR,
    FLASH_TIMEOUT,
	FLASH_PG_ERROR,
	FLASH_WRP_ERROR,
	FLASH_VERIFY_ERROR
}flash_status_t;


flash_status_t flash_erase(uint32_t start_address, uint32_t size_bytes);

flash_status_t flash_program(uint32_t flash_address, uint16_t *chunk_buffer, uint32_t halfword_count);

#endif /* FLASH_H_ */
