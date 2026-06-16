/*
 * firmware_receiver.c
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#include "uart.h"
#include "firmware_receiver.h"
#include "flash_driver.h"
#include <stdio.h>

#define FLASH_PAGE_SIZE		0x400U	// 1KB

/*------------ For Erase Flash -------------*/
flash_status_t erase_flash_region(uint32_t start_address, uint32_t size_bytes) {
	flash_status_t status;
	status = flash_unlock();
	if (status != FLASH_OK) {
		flash_lock();
		println("Flash unlocked failed");
		return status;
	}

	/*-------------- Dynamic Flash Erased -------------*/
	uint32_t number_of_pages_to_erase;

	number_of_pages_to_erase = ((size_bytes + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE);

	for (uint32_t page_index = 0; page_index < number_of_pages_to_erase; page_index++) {
		status = flash_erase_page(start_address + (page_index * FLASH_PAGE_SIZE));

		if (status != FLASH_OK) {
			flash_lock();
			return status;
		}
	}

	flash_lock();
	return FLASH_OK;
}

/*------------ For Storing Chunk Data in Flash -------------*/
flash_status_t program_flash_chunk(uint32_t flash_address,
		uint16_t *chunk_buffer, uint32_t halfword_count)
	{
	flash_status_t status;
	status = flash_unlock();
	if (status != FLASH_OK) {
		flash_lock();
		println("Flash unlocked failed");
		return status;
	}



	status = flash_program_buffer(flash_address, chunk_buffer, halfword_count);
	if (status != FLASH_OK)
	{
		flash_lock();
		println("Flash Write Failed");
		return status;
	}



	/***** Read the stored data from flash and compare *****/
	for (uint32_t i = 0; i < halfword_count; i++)
	{
		uint16_t flash_value = *(__IO uint16_t*) (flash_address + (i * 2));

		if (flash_value != chunk_buffer[i]) {
			return FLASH_VERIFY_ERROR;
		}
	}

	flash_lock();
	return FLASH_OK;
}
