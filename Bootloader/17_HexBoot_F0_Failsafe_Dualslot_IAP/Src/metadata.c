/*
 * metadata.c
 *
 * Module Responsibilities
 *
 * - Initialize firmware metadata
 * - Read metadata from Flash
 * - Write metadata to Flash
 * - Verify metadata integrity
 * - Manage active application slot
 * ======================================
 *  Created on: 26-May-2026
 *      Author: brajo
 */
#include "metadata.h"
#include "crc.h"

metadata_t metadata;

/*********************************
         Private Function
 ********************************/


static void metadata_default_values(void)
{
	metadata.magic = METADATA_MAGIC;
	metadata.active_slot = SLOT_A;
	metadata.boot_state = BOOT_NORMAL;
	metadata.boot_count = 0;

    /* Slot A */
    metadata.slot_a.version   = 0;
    metadata.slot_a.size      = 0;
    metadata.slot_a.crc       = 0;
    metadata.slot_a.is_valid  = 0;
    metadata.slot_a.confirmed = 0;
    metadata.slot_a.reserved[0]  = 0;
    metadata.slot_a.reserved[1]  = 0;

    /* Slot B */
    metadata.slot_b.version   = 0;
    metadata.slot_b.size      = 0;
    metadata.slot_b.crc       = 0;
    metadata.slot_b.is_valid  = 0;
    metadata.slot_b.confirmed = 0;
    metadata.slot_b.reserved[0]  = 0;
    metadata.slot_b.reserved[1]  = 0;

    metadata.metadata_crc = 0;
}



/********************************
         Public Function
 ********************************/

/******************************************************************************
 * Initialize Bootloader Metadata
 *
 * Workflow:
 *  1. Load metadata from Flash.
 *  2. Verify metadata integrity.
 *  3. If invalid:
 *      - Populate default metadata.
 *      - Save metadata to Flash.
 *
 * This function only writes to Flash when metadata is missing or corrupted.
 ******************************************************************************/
void metadata_init(metadata_t *metadata){

	metadata_load(metadata);

	if(!metadata_verify(metadata)){

		metadata_default_values();
		metadata_save(metadata);

	}
}



void metadata_load(metadata_t *metadata){
	const metadata_t *flash_metadata = ((const metadata_t*) METADATA_ADDRESS);
	*metadata = *flash_metadata;
}



/******************************************************************************
 * Save metadata to Flash.
 *
 * Calculates the metadata CRC and stores the metadata structure in the
 * dedicated metadata Flash page.
 *
 * Parameters:
 *   metadata - Pointer to the metadata structure to be stored.
 *
 * Returns:
 *   FLASH_OK on success, otherwise an appropriate flash_status_t error code.
 ******************************************************************************/
flash_status_t metadata_save(metadata_t *metadata)
{
	metadata->metadata_crc = 0;
	/*
	 * Calculate metadata CRC.
	 *
	 * The metadata_crc field is cleared before calculation so it
	 * is excluded from the CRC result.
	 */
	metadata->metadata_crc = crc16_calculate((uint8_t*) metadata, sizeof(metadata_t));

	flash_status_t flash_status;

	flash_status = flash_erase(METADATA_ADDRESS, sizeof(metadata_t));

	if(flash_status != FLASH_OK){
		return flash_status;
	}

	uint32_t halfword_count = sizeof(metadata_t) / sizeof(uint16_t);
	flash_status = flash_program(METADATA_ADDRESS, (uint16_t*) metadata, halfword_count);
	if(flash_status != FLASH_OK){
		return flash_status;
	}

	return FLASH_OK;
}



uint8_t metadata_verify(const metadata_t *metadata){



    /* Verify magic number */
	if(metadata->magic != METADATA_MAGIC)
	{
		return 0;
	}


    /* Verify metadata CRC */
	metadata_t temp = *metadata;

	uint32_t stored_crc = metadata->metadata_crc;

	temp.metadata_crc = 0;

	uint32_t calculated_crc = crc16_calculate((uint8_t*)&temp, sizeof(metadata_t));
	if(stored_crc != calculated_crc)
	{
		return 0;
	}


    /* Verify active slot */
	if(metadata->active_slot != SLOT_A && metadata->active_slot != SLOT_B)
	{
	    return 0;
	}


    /* Verify boot state */
	if(metadata->boot_state > BOOT_ERROR)
	{
	    return 0;
	}

	return 1;
}



slot_t metadata_get_active_slot(const metadata_t *metadata){
	return metadata->active_slot;
}



void metadata_set_active_slot(metadata_t *metadata, slot_t slot){
	metadata->active_slot = slot;
}
