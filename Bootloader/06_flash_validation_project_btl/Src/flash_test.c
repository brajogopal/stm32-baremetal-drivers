/*
 * flash_test.c
 *
 *  Created on: 17-May-2026
 *      Author: brajo
 */

#include "stm32f030x8.h"
#include "flash_test.h"
#include "flash_driver.h"
#include "uart.h"

#define TEST_FLASH_PAGE_ADDR  0x0800FC00
#define TEST_FLASH_ALIGNMENT  0x0800FC01

uint16_t test_data[] =
{
    0x1234,
    0x5678,
    0xABCD,
    0x5555,
    0xAAAA
};


void test_flash_erase(void){
	flash_unlock();
	println("Flash Unlocked");

	if(flash_erase_page(TEST_FLASH_PAGE_ADDR) != FLASH_OK){
		println("Flash Erase Failed");
		flash_lock();
		println("Flash locked");
		return;
	}

	println("Flash Erased");
	println("checking page...");

	for(int i = 0; i < 512; i++){
		if(*(__IO uint16_t*)(TEST_FLASH_PAGE_ADDR + (i * 2)) != 0xFFFF)
		{
			println("Erase verification failed");
			flash_lock();
			println("Flash locked");
			return;
		}
	}

	println("Erase verification success");

	flash_lock();
	println("Flash locked");
}


void test_flash_program(void){
	flash_unlock();
	println("Flash Unlocked");

	flash_status_t status;
	println("Writing on the flash");

	status = flash_program_buffer( TEST_FLASH_PAGE_ADDR, test_data, 5);

	if(status != FLASH_OK)
	{
	    println("Buffer Programming Failed");

	    flash_lock();
	    println("Flash Locked");

	    return;
	}

	flash_lock();
	println("Flash locked");
}


void test_alignment_error(void){
	flash_unlock();
	println("Flash Unlocked");

	flash_status_t status;
	println("Writing on the flash");

	status = flash_program_buffer( TEST_FLASH_ALIGNMENT, test_data, 5);

	if(status != FLASH_OK)
	{
	    println("Alignment Protection PASS");

	    flash_lock();
	    println("Flash Locked");

	    return;
	}	else	{
		println("Alignment Protection FAIL");
	}

	flash_lock();
	println("Flash locked");
}



void test_locked_flash(void){

	flash_status_t status;
	println("Writing on the flash");

	status = flash_program_buffer( TEST_FLASH_PAGE_ADDR, test_data, 5);

	if(status == FLASH_ERROR)
	{
	    println("FLASH_ERROR : Lock Protection PASS");

	    flash_lock();
	    println("Flash Locked");

	    return;
	}	else if(status == FLASH_PG_ERROR){
		 println("FLASH PG ERROR");

		 flash_lock();
		 println("Flash Locked");
		 return;
	}
		else if(status == FLASH_OK){
		 println("Lock Protection FAIL");
	}

	flash_lock();
	println("Flash locked");
}


void test_pg_error(void){
	flash_status_t status;

	flash_unlock();

	if(flash_erase_page(TEST_FLASH_PAGE_ADDR) != FLASH_OK){
		println("Flash Erase Failed");
		flash_lock();
		return;
	}

	if(flash_program_buffer(TEST_FLASH_PAGE_ADDR, test_data, 5) != FLASH_OK){
		println("Initial program failed");
		flash_lock();
		return;
	}

	println("Programming without erase");
	status = flash_program_buffer(TEST_FLASH_PAGE_ADDR, test_data, 5);

	if(status == FLASH_PG_ERROR){
		println("PGERR Protection PASS");
	}	else if (status == FLASH_ERROR){
		println("FLASH_ERROR Triggered");
	}	else	{
		println("PGERR Protection FAIL");
	}
	flash_lock();
}


void flash_test_run(void){
	test_pg_error();
}
