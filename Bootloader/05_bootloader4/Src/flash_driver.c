/*
 * flash_driver.c
 *
 *  Created on: 12-May-2026
 *      Author: brajo
 */
#include "stm32f030x8.h"
#include	"flash_driver.h"


#define FLASH_FKEY1		0x45670123
#define FLASH_FKEY2		0xCDEF89AB
#define FLASH_LOCK		(1U << 7)
#define FLASH_PER		(1U << 1)
#define FLASH_STRT		(1U << 6)
#define FLASH_PG		(1U << 0)
#define STATUS_EOP		(1U << 5)
#define STATUS_BSY		(1U << 0)

static flash_status_t flash_wait_busy(void){
	while(FLASH->SR & STATUS_BSY){}
	return FLASH_OK;
}

flash_status_t flash_unlock(void){
	/* (1) Wait till no operation is on going */
	flash_wait_busy();

	/* (2) Check that the flash memory is unlocked */
	if ((FLASH->CR & FLASH_LOCK) != 0)
	{

	/* (3) Perform unlock sequence */
	FLASH->KEYR = FLASH_FKEY1;
	FLASH->KEYR = FLASH_FKEY2;
	}
}

flash_status_t flash_lock(void){
	FLASH->CR |= FLASH_LOCK;
}

flash_status_t flash_erase_page(uint32_t addr){
	if ((FLASH->CR & FLASH_LOCK) == 0){

		flash_wait_busy();
		
		FLASH->CR |= FLASH_PER;

		FLASH->AR = addr;

		FLASH->CR |= FLASH_STRT;

		flash_wait_busy();

		if(FLASH->SR & STATUS_EOP)
		{
		FLASH->SR |= STATUS_EOP;
		}

		FLASH->CR &= ~FLASH_PER;

	}	else	{
		return FLASH_ERROR;
	}
}


flash_status_t flash_program_halfword(uint32_t addr,uint16_t data){
	FLASH->CR |= FLASH_PG;
	*(__IO uint16_t*)(addr) = data;
	flash_wait_busy();
	if ((FLASH->SR & STATUS_EOP) != 0)
	{
	  FLASH->SR |= STATUS_EOP; 
	}	else	{
		return FLASH_ERROR;
	}
	FLASH->CR &= ~FLASH_PG;
}

flash_program_buffer(addr, data, length){
	uint32_t buffer[] = data;
	for(int i = 0, i < length, i++){
		flash_program_halfword(buffer[i], data);
	}
}
