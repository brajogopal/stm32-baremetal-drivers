/*
 * metadata.c
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */
#include "metadata.h"
#include "flash_driver.h"


void metadata_read(metadata_slot_t slot,firmware_metadata_t *metadata)
{
	uint32_t address;

	    switch(slot)
	    {
	        case METADATA_SLOT_A:
	            address = METADATA_A_ADDRESS;
	            break;

	        case METADATA_SLOT_B:
	            address = METADATA_B_ADDRESS;
	            break;

	        default:
	            return;
	    }

	*metadata = *(firmware_metadata_t*) address;
}

flash_status_t metadata_write(metadata_slot_t slot, firmware_metadata_t *metadata)
{
	uint32_t address;

	    switch(slot)
	    {
	        case METADATA_SLOT_A:
	            address = METADATA_A_ADDRESS;
	            break;

	        case METADATA_SLOT_B:
	            address = METADATA_B_ADDRESS;
	            break;

	        default:
	            return FLASH_ERROR;
	    }

	flash_status_t status;
	status = flash_unlock();
	if(status != FLASH_OK){
		return status;
	}

	status = flash_program_buffer(address , (uint16_t*)metadata, sizeof(firmware_metadata_t)/2);
		if(status != FLASH_OK){

			flash_lock();
			return status;
		}

		flash_lock();
		return status;
}
