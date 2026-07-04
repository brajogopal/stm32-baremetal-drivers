/****************************************************
 * slot_manager.h
 *
 * Module Responsibilities
 *
 * - Manage firmware slots
 * - Determine active/inactive slot
 * - Provide slot information
 * - Select boot target
 *
 * ==================================
 *  Created on: 04-Jul-2026
 *      Author: brajo
 ***************************************************/

#ifndef SLOT_MANAGER_H_
#define SLOT_MANAGER_H_

#include "metadata.h"

slot_t slot_manager_get_active_slot(const metadata_t *metadata);

slot_t slot_manager_get_inactive_slot(const metadata_t *metadata);

uint32_t slot_manager_get_slot_address(slot_t slot);

slot_info_t *slot_manager_get_slot_info(metadata_t *metadata,slot_t slot);

uint8_t slot_manager_is_slot_valid(metadata_t *metadata, slot_t slot);

slot_t slot_manager_select_boot_slot(metadata_t *metadata);

#endif /* SLOT_MANAGER_H_ */
