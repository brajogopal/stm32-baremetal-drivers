/*
 * metadata.h
 *
 * Public interface for firmware metadata management.
 */

#ifndef METADATA_H_
#define METADATA_H_

#include <stdint.h>
#include "flash.h"

#define METADATA_ADDRESS	0x08004000U	//1KB
#define SLOT_A_ADDRESS		0x08004400U	//23KB
#define SLOT_B_ADDRESS		0x0800A000U	//23KB

#define APP_MAGIC			0xDEADBEEF
#define METADATA_MAGIC		0x48455842



typedef enum {
    SLOT_A = 0,
    SLOT_B = 1,
	SLOT_NONE
} slot_t;

typedef enum {
    BOOT_NORMAL = 0,
    BOOT_UPDATE_PENDING,
    BOOT_ROLLBACK,
    BOOT_ERROR
} boot_state_t;



typedef struct
{
    uint32_t version;
    uint32_t size;
    uint32_t crc;
    uint8_t  is_valid;
    uint8_t  confirmed;
    uint8_t  reserved[2];
} slot_info_t;



typedef struct
{
	/* Boot Information */
    uint32_t magic;         	// Metadata validity
    slot_t  active_slot;   		// A or B
    boot_state_t  boot_state;   // Normal, Update Pending, Rollback
    uint16_t boot_count;     	// Failed boot counter

    /* Slot Information */
    slot_info_t slot_a;
    slot_info_t slot_b;

    /* Metadata */
    uint32_t metadata_crc;

} metadata_t;



extern metadata_t metadata;



void metadata_init(metadata_t *metadata);

void metadata_load(metadata_t *metadata);

flash_status_t metadata_save(metadata_t *metadata);

uint8_t metadata_verify(const metadata_t *metadata);

slot_t metadata_get_active_slot(const metadata_t *metadata);

void metadata_set_active_slot(metadata_t *metadata, slot_t slot);

#endif /* METADATA_H_ */
