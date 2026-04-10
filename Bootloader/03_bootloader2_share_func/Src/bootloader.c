/*
 * bootloader.c
 *
 *  Created on: 10-Apr-2026
 *      Author: BROJOGOPAL
 */
#include "stm32f030x8.h"
#include "bootloader.h"



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


