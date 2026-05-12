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
    FLASH_TIMEOUT
}flash_status_t;

flash_status_t flash_unlock(void);
flash_status_t flash_lock(void);
flash_status_t flash_erase_page(uint32_t addr);
flash_status_t flash_program_halfword(uint32_t addr,uint16_t data);

#endif /* FLASH_DRIVER_H_ */
