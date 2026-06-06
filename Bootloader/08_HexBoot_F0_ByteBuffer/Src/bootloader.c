/*
 * bootloader.c
 *
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */
#include "bootloader.h"
#include "stm32f030x8.h"
#include "uart.h"
#include "delay.h"
#include "flash_driver.h"
#include <stdio.h>

#define SYSCFG_EN (1U<<0)


/*---------------------------------------------------
 *
 * Reserved for Project 10+ architecture.
 * Unused in this project.
 * Kept for future bootloader status handling.
 *
static void flash_handle_status(flash_status_t status)
{
	switch(status)
	{
	case FLASH_OK:
		println("FLASH_OK");
		break;

	case FLASH_ERROR:
		println("FLASH_ERROR");
		break;

	case FLASH_TIMEOUT:
		println("FLASH_TIMEOUT");
		break;

	default:
		println("UNKNOWN_FLASH_STATE");
	}
}
-----------------------------------------------*/


void jmp_to_default_app(void) {
	uint32_t app_start_address;
	func_ptr jump_to_app;
	println("bootloader started..");
	delay_ms(300);

	if (is_valid_app(APPLICATION_ADDRESS)) {

		__disable_irq();
		relocate_vector_table();

		/*Initialize main stack pointer */
		__set_MSP(*(uint32_t*) APPLICATION_ADDRESS);

		app_start_address = *(uint32_t*) (APPLICATION_ADDRESS + 4);
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

	// MSP must be in SRAM
	if (msp < SRAM_START || msp > SRAM_END)
		return 0;

	// MSP must be aligned to 32 bit(4 byte)
	if (msp % 4)
		return 0;

	// Reset handler must be in Flash
	if (reset < FLASH_START || reset > FLASH_END)
		return 0;

	// Thumb bit check(Cortex-M supports only Thumb mode, Function address must have LSB=1)
	if ((reset & 1) == 0)
		return 0;

	return 1;
}



void relocate_vector_table(void) {
	uint32_t *src = (uint32_t*) APPLICATION_ADDRESS;
	uint32_t *dst = (uint32_t*) SRAM_START;

	for (int i = 0; i < VECTOR_COUNT; i++) {   // copying of vectors
		dst[i] = src[i];
	}

	RCC->APB2ENR |= SYSCFG_EN;
	SYSCFG->CFGR1 &= ~(3 << 0);
	SYSCFG->CFGR1 |= (3 << 0);   // SRAM mapped at 0x00000000

	__DSB();   // Data Synchronization Barrier
	__ISB();   // Instruction Synchronization Barrier
}

