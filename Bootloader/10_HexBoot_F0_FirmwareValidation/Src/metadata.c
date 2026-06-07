/*
 * metadata.c
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */
#include "metadata.h"
#include "flash_driver.h"


void metadata_read(firmware_metadata_t *metadata)
{
	*metadata = *(firmware_metadata_t*)METADATA_A_ADDRESS;
}

flash_status_t metadata_write(firmware_metadata_t *metadata)
{
	flash_status_t status;
	status = flash_unlock();
	if(status != FLASH_OK){
		return status;
	}

	status = flash_program_buffer(METADATA_A_ADDRESS , (uint16_t*)metadata, sizeof(firmware_metadata_t)/2);
		if(status != FLASH_OK){

			flash_lock();
			return status;
		}

		flash_lock();
		return status;
}
