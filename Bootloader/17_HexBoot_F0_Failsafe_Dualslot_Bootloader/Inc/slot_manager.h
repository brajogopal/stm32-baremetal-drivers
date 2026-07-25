/*
 * slot_manager.h
 *
 * Public interface for firmware slot management.
 */

#ifndef SLOT_MANAGER_H_
#define SLOT_MANAGER_H_

#include "metadata.h"

typedef enum
{
    SLOT_BOOTABLE,

    SLOT_INVALID_SIZE,

    SLOT_INVALID_VERSION,

    SLOT_INVALID_METADATA,

    SLOT_INVALID_VECTOR,

    SLOT_INVALID_RESET_HANDLER,

    SLOT_NOT_CONFIRMED

} slot_status_t;

slot_t slot_manager_get_active_slot(const metadata_t *metadata);

slot_t slot_manager_get_inactive_slot(const metadata_t *metadata);

uint32_t slot_manager_get_slot_address(slot_t slot);

slot_info_t *slot_manager_get_slot_info(metadata_t *metadata,slot_t slot);

slot_status_t slot_manager_is_bootable(metadata_t *metadata, slot_t slot);

slot_t slot_manager_select_boot_slot(metadata_t *metadata);

#endif /* SLOT_MANAGER_H_ */
