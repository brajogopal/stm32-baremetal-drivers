/*
 * storage_manager.c
 *
 *  Created on: 19-May-2026
 *      Author: brajo
 */

#include "storage_manager.h"
#include "flash_driver.h"



flash_status_t storage_save(storage_data_t *data){

	flash_status_t status;


	status = flash_unlock();
	if(status != FLASH_OK){
		flash_lock();
		return status;
	}


	status = flash_erase_page(STORAGE_PAGE_ADDR);
	if(status != FLASH_OK){
		flash_lock();
		return status;
	}


	status = flash_program_buffer(STORAGE_PAGE_ADDR, (uint16_t*)data ,sizeof(storage_data_t) / 2);
	if(status != FLASH_OK){
		flash_lock();
		return status;
		}


	flash_lock();

	return status;

}
