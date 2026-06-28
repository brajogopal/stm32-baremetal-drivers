/*
 * flash_driver.h
 *
 *  Created on: 12-May-2026
 *      Author: brajo
 */

#ifndef FLASH_DRIVER_H_
#define FLASH_DRIVER_H_
#include <stdio.h>
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


flash_status_t flash_unlock(void);
flash_status_t flash_lock(void);
flash_status_t flash_erase_page(uint32_t addr);
flash_status_t erase_flash_region(uint32_t start_address, uint32_t size_bytes);

flash_status_t flash_program_halfword(uint32_t addr,uint16_t data);
flash_status_t flash_program_buffer(uint32_t addr,uint16_t *data, uint32_t length);
flash_status_t program_flash_chunk(uint32_t flash_address, uint16_t *chunk_buffer, uint32_t halfword_count);
#endif /* FLASH_DRIVER_H_ */
