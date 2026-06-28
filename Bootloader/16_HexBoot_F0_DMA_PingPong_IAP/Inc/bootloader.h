/*
 * bootloader.h
 *
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>
#include "flash_driver.h"

#define VECTOR_COUNT 				48

#define SRAM_START					0x20000000U
#define SRAM_SIZE  (8 * 1024)					//For STM32F030C8TX
#define SRAM_END   (SRAM_START + SRAM_SIZE)

#define FLASH_START 0x08000000U
#define FLASH_END   (FLASH_START + (64 * 1024))	//For STM32F030C8TX

typedef void (*func_ptr)(void);
int is_valid_app(uint32_t addr);
void relocate_vector_table(uint32_t app_address);
void jmp_to_app(uint32_t app_address);
void flash_handle_status(flash_status_t status);


#endif /* BOOTLOADER_H_ */
