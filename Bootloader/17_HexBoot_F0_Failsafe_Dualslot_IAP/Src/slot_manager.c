/*
 * slot_manager.c
 *
 *  Created on: 04-Jul-2026
 *      Author: brajo
 */
#include "slot_manager.h"


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
        return APPLICATION_A_ADDRESS;
    }

    return APPLICATION_B_ADDRESS;
}

slot_info_t *slot_manager_get_slot_info(metadata_t *metadata,slot_t slot){
	if(slot == SLOT_A){
		return &metadata->slot_a;
	}
	return &metadata->slot_b;
}
