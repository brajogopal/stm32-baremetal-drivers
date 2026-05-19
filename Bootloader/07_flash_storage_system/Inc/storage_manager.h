/*
 * storage_manager.h
 *
 *  Created on: 19-May-2026
 *      Author: brajo
 */

#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

#define STORAGE_PAGE_ADDR   0x0800F800
#define STORAGE_MAGIC       0xDEADBEEF

typedef struct
{
    uint32_t magic_number;

    uint32_t boot_count;

    uint32_t firmware_version;

} storage_data_t;

void storage_save(storage_data_t *data);

void storage_load(storage_data_t *data);

#endif /* STORAGE_MANAGER_H_ */
