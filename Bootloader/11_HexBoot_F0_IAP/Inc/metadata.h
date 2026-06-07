/*
 * metadata.h
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */

#ifndef METADATA_H_
#define METADATA_H_

#include <stdint.h>
#include "flash_driver.h"

#define METADATA_A_ADDRESS     0x08004000U
#define APPLICATION_A_ADDRESS  0x08004020U	//24KB

#define METADATA_B_ADDRESS     0x0800A000U
#define APPLICATION_B_ADDRESS  0x0800A020U	//24KB

#define APP_MAGIC 0xDEADBEEF

typedef struct
{
    uint32_t magic_number;
    uint32_t firmware_length;
    uint16_t firmware_crc;
    uint16_t reserved;
} firmware_metadata_t;

typedef enum
{
    METADATA_SLOT_A = 0,
    METADATA_SLOT_B
} metadata_slot_t;


flash_status_t metadata_write(metadata_slot_t slot, firmware_metadata_t *metadata);
void metadata_read(metadata_slot_t slot, firmware_metadata_t *metadata);

#endif /* METADATA_H_ */
