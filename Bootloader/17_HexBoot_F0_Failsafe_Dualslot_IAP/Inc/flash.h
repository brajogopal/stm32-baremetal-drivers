/*
 * flash_driver.h
 *
 * Public interface for internal Flash memory operations.
 *
 * Responsibilities:
 * - Flash erase.
 * - Flash programming.
 * - Flash verification.
 *
 * All Flash operations are performed using register-level programming.
 *
 * =====================================================================
 *  Created on: 12-May-2026
 *      Author: brajo
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
