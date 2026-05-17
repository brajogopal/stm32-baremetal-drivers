/*
 * flash_driver.h
 *
 *  Created on: 12-May-2026
 *      Author: brajo
 */

#ifndef FLASH_DRIVER_H_
#define FLASH_DRIVER_H_

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
flash_status_t flash_program_halfword(uint32_t addr,uint16_t data);

flash_status_t flash_program_buffer(uint32_t addr,uint16_t *data, uint32_t length);

#endif /* FLASH_DRIVER_H_ */
