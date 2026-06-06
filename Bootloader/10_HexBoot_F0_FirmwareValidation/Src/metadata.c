/*
 * metadata.c
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */
#include "metadata.h"


flash_status_t metadata_read(firmware_metadata_t *metadata)
{
	*metadata = *(firmware_metadata_t*)METADATA_A_ADDRESS;
}

flash_status_t metadata_write(firmware_metadata_t *metadata)
{

}
