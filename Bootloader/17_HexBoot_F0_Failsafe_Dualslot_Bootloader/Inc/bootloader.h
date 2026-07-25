/*
 * bootloader.h
 *
 * Public interface for the bootloader.
 *
 * Responsibilities:
 * - Validate application.
 * - Configure application stack.
 * - Jump to user application.
 * - Bootloader startup sequence.
 *
 * ==================================
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

#include <flash.h>


#define VECTOR_COUNT 			48

#define SRAM_START				0x20000000U
#define SRAM_SIZE  				(8 * 1024)	//For STM32F030C8TX
#define SRAM_END  				(SRAM_START + SRAM_SIZE)

#define FLASH_START				0x08000000U
#define FLASH_END   			(FLASH_START + (64 * 1024))	//For STM32F030C8TX


typedef struct
{
    uint16_t payload_length;
    uint16_t crc;
} firmware_info_t;


void bootloader_init(void);
void bootloader_run(void);


#endif /* BOOTLOADER_H_ */
