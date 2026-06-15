/*
 * bootloader.c
 *
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */
#include "bootloader.h"
#include "stm32f030x8.h"
#include "metadata.h"
#include "uart.h"
#include "delay.h"
#include "flash_driver.h"
#include <stdio.h>

#define SYSCFG_EN (1U<<0)



void flash_handle_status(flash_status_t status)
{
	switch(status)
	{
	case FLASH_OK:
		println("FLASH_OK");
		break;

	case FLASH_ERROR:
		flash_lock();
		println("FLASH_ERROR");
		break;

	case FLASH_TIMEOUT:
		flash_lock();
		println("FLASH_TIMEOUT");
		break;

	default:
		println("UNKNOWN_FLASH_STATE");
	}
}





void jmp_to_app(uint32_t app_address) {
	func_ptr jump_to_app;
	println("bootloader started..");
	delay_ms(300);

	if (is_valid_app(app_address)) {

		__disable_irq();
		relocate_vector_table(app_address);

		/*Initialize main stack pointer */
		__set_MSP(*(uint32_t*) app_address);

		uint32_t app_start_address = *(uint32_t*) (app_address + 4);
		jump_to_app = (func_ptr) (app_start_address);
		println("valid application present..");

		/*jump*/
		jump_to_app();

		while (1);//Fail-safe design(If execution ever comes back, CPU gets stuck here safely)
	} else {
		println("No valid application found");
	}
}

int is_valid_app(uint32_t addr) {
	uint32_t msp = *(uint32_t*) addr;
	uint32_t reset = *(uint32_t*) (addr + 4);


	firmware_metadata_t metadata;
	if(addr == APPLICATION_A_ADDRESS){
	metadata_read(METADATA_SLOT_A,&metadata);
	}
	else if(addr == APPLICATION_B_ADDRESS){
	metadata_read(METADATA_SLOT_B,&metadata);
	}
	else	{
		println("Invalid Application Address");
		 return 0;
	}

	// MSP must be in SRAM
	if (msp < SRAM_START || msp > SRAM_END){
		return 0;
	}

	// MSP must be aligned to 32 bit(4 byte)
	if (msp % 4){
		return 0;
	}
	// Reset handler must be in Flash
	if (reset < FLASH_START || reset > FLASH_END){
		return 0;
	}
	// Thumb bit check(Cortex-M supports only Thumb mode, Function address must have LSB=1)
	if ((reset & 1) == 0){
		return 0;
	}
	if(metadata.magic_number != APP_MAGIC){
		println("Invalid APP_MAGIC");
		return 0;
	}
	return 1;
}



void relocate_vector_table(uint32_t app_address) {
	uint32_t *src = (uint32_t*) app_address;
	uint32_t *dst = (uint32_t*) SRAM_START;

	for (uint32_t i = 0; i < VECTOR_COUNT; i++) {   // copying of vectors
		dst[i] = src[i];
	}

	RCC->APB2ENR |= SYSCFG_EN;
	SYSCFG->CFGR1 &= ~(3 << 0);
	SYSCFG->CFGR1 |= (3 << 0);   // SRAM mapped at 0x00000000

	__DSB();   // Data Synchronization Barrier
	__ISB();   // Instruction Synchronization Barrier
}

