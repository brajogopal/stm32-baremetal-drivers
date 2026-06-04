/*
 * metadata.h
 *
 *  Created on: 26-May-2026
 *      Author: brajo
 */

#ifndef METADATA_H_
#define METADATA_H_

#include "flash_driver.h"
#include "storage_manager.h"
#include "stm32f030x8.h"
#include <stdint.h>



void menu(void);
void view_metadata(storage_data_t *data);
void update_metadata(storage_data_t *data);
void condition_check(void);

#endif /* METADATA_H_ */
