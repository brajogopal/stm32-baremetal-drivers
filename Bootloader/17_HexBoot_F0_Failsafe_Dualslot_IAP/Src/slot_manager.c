/*
 * slot_manager.c
 *
 *  Created on: 04-Jul-2026
 *      Author: brajo
 */
#include "slot_manager.h"
#include "uart.h"

slot_t slot_manager_get_active_slot(const metadata_t *metadata){
	return metadata_get_active_slot(metadata);
}




slot_t slot_manager_get_inactive_slot(const metadata_t *metadata){
	if(metadata_get_active_slot(metadata) == SLOT_A){
		return SLOT_B;
	}	else	{
		return SLOT_A;
	}
}




uint32_t slot_manager_get_slot_address(slot_t slot){
    if(slot == SLOT_A)
    {
        return SLOT_A_ADDRESS;
    }

    return SLOT_B_ADDRESS;
}




slot_info_t *slot_manager_get_slot_info(metadata_t *metadata,slot_t slot){
	if(slot == SLOT_A){
		return &metadata->slot_a;
	}
	return &metadata->slot_b;
}



slot_status_t slot_manager_is_bootable(metadata_t *metadata, slot_t slot){
	if(slot != SLOT_A && slot != SLOT_B){
		println("unknown slot");
		return SLOT_NOT_CONFIRMED;
	}

	slot_info_t *slot_info = slot_manager_get_slot_info(metadata, slot);

	if(slot_info == 0)
	{
		println("slot_info empty");
	    return SLOT_INVALID_METADATA;
	}

	if(slot_info->is_valid == 0){
		println("IS VALID");
		return SLOT_INVALID_METADATA;
	}

	if(slot_info->version == 0)
	{
		println("version empty");
	    return SLOT_INVALID_VERSION;
	}

	if(slot_info->size == 0)
	{
		println("size empty");
	    return SLOT_INVALID_SIZE;
	}

	if(slot_info->crc == 0)
	{
		println("crc empty");
	    return SLOT_INVALID_METADATA;
	}

	return SLOT_BOOTABLE;

}


slot_t slot_manager_select_boot_slot(metadata_t *metadata){

	slot_t slot = slot_manager_get_active_slot(metadata);

	if(slot_manager_is_bootable(metadata, slot) == SLOT_BOOTABLE){
		return slot;
	}

	println("active slot is not bootable");

	slot =  slot_manager_get_inactive_slot(metadata);

	if(slot_manager_is_bootable(metadata, slot) == SLOT_BOOTABLE){
		return slot;
	}

	println("inactive slot is also not bootable");

	return SLOT_NONE;
}
