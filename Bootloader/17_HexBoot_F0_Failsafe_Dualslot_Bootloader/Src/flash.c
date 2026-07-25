/*
 * flash.c
 *
 * Internal Flash memory driver.
 */
#include "stm32f030x8.h"
#include "flash.h"
#include "uart.h"


#define FLASH_FKEY1		0x45670123
#define FLASH_FKEY2		0xCDEF89AB
#define FLASH_LOCK		(1U << 7)
#define FLASH_PER		(1U << 1)
#define FLASH_STRT		(1U << 6)
#define FLASH_PG		(1U << 0)

#define STATUS_BSY		(1U << 0)
#define STATUS_PGERR	(1U << 2)
#define STATUS_WRPRTERR	(1U << 4)
#define STATUS_EOP		(1U << 5)
#define FLASH_ALL_ERRORS (STATUS_EOP | STATUS_PGERR | STATUS_WRPRTERR)
#define FLASH_PAGE_SIZE		0x400U	// 1KB
#define FLASH_TIMEOUT_VALUE		100000U



/******************************************************************************
                     Private Function Prototypes
 ******************************************************************************/
static flash_status_t flash_unlock(void);
static flash_status_t flash_lock(void);
static void flash_clear_flags(void);
static flash_status_t flash_wait_busy(void);
static flash_status_t flash_erase_page(uint32_t addr);
static flash_status_t flash_program_halfword(uint32_t addr, uint16_t data);
static flash_status_t flash_program_buffer(uint32_t addr, uint16_t *data, uint32_t length);



/******************************************************************************
                          Private Functions
 ******************************************************************************/

static flash_status_t flash_unlock(void)
{
	/* (1) Wait till no operation is on going */
	if(flash_wait_busy() != FLASH_OK){
		return FLASH_TIMEOUT;
	}

	/* (2) Check that the flash memory is unlocked */
	if ((FLASH->CR & FLASH_LOCK) != 0)
	{

	/* (3) Perform unlock sequence */
	FLASH->KEYR = FLASH_FKEY1;
	FLASH->KEYR = FLASH_FKEY2;
	}
	return FLASH_OK;
}


static flash_status_t flash_lock(void)
{
	FLASH->CR |= FLASH_LOCK;
	return FLASH_OK;
}



static void flash_clear_flags(void)
{
	FLASH->SR |= FLASH_ALL_ERRORS;
}


static flash_status_t flash_wait_busy(void)
{

	uint32_t timeout = FLASH_TIMEOUT_VALUE;

	while(FLASH->SR & STATUS_BSY){

	timeout --;
	if(timeout == 0){
		return FLASH_TIMEOUT;
	}
	}
	return FLASH_OK;
}


static flash_status_t flash_erase_page(uint32_t addr)
{

	flash_clear_flags();

	if ((FLASH->CR & FLASH_LOCK) == 0){

		if(flash_wait_busy() != FLASH_OK){
			return FLASH_TIMEOUT;
		}
		
		FLASH->CR |= FLASH_PER;

		FLASH->AR = addr;

		FLASH->CR |= FLASH_STRT;

		if(flash_wait_busy() != FLASH_OK){
			return FLASH_TIMEOUT;
		}

		FLASH->CR &= ~FLASH_PER;

		if(FLASH->SR & STATUS_WRPRTERR){
			FLASH->SR |= STATUS_WRPRTERR;
			return FLASH_WRP_ERROR;
		}

		if(FLASH->SR & STATUS_EOP)
		{
			flash_clear_flags();
		}	else	{
				return FLASH_ERROR;
		}

		return FLASH_OK;

	}	else	{
		return FLASH_ERROR;
	}
}


static flash_status_t flash_program_halfword(uint32_t addr,uint16_t data)
{

	if((addr % 2) || (*(__IO uint16_t*)addr != 0xFFFF)) //Check 16bit DATA Alignment & Address Erased properly
	{
	    return FLASH_ERROR;
	}

	if((FLASH->CR & FLASH_LOCK) == 0){

		FLASH->CR |= FLASH_PG;

		*(__IO uint16_t*)(addr) = data;

		if(flash_wait_busy() != FLASH_OK){
			return FLASH_TIMEOUT;
		}

		FLASH->CR &= ~FLASH_PG;

		if(FLASH->SR & STATUS_WRPRTERR){
			FLASH->SR |= STATUS_WRPRTERR;
			return  FLASH_WRP_ERROR;
		}
		else if (FLASH->SR & STATUS_PGERR)
		{
			FLASH->SR |= STATUS_PGERR;
			return FLASH_PG_ERROR;
		}

		if (FLASH->SR & STATUS_EOP)
		{
			flash_clear_flags();
		}
		else
		{
			return FLASH_ERROR;
		}
		return FLASH_OK;
	}
	else
	{
		return FLASH_ERROR;
	}
}


static flash_status_t flash_program_buffer(uint32_t addr,uint16_t *data, uint32_t length)
{
	flash_clear_flags();
	for(uint32_t i = 0; i < length; i++)
	{
		flash_status_t status;

		status = flash_program_halfword(addr + (i * 2), data[i]);

		if(status != FLASH_OK)
		{
		    return status;
		}

		const uint16_t value = *(__IO uint16_t*)(addr + (i * 2));
		if(value != data[i])
		{
			return FLASH_VERIFY_ERROR;
		}
	}
	return FLASH_OK;
}


/******************************************************************************
 * Public Functions
 ******************************************************************************/

/*
 * Erase Flash pages.
 */
flash_status_t flash_erase(uint32_t start_address, uint32_t size_bytes)
{
	flash_status_t status;
	status = flash_unlock();
	if (status != FLASH_OK)
	{
		flash_lock();
		println("Flash unlock failed");
		return status;
	}

	/* Dynamic Flash Erased */
	uint32_t number_of_pages_to_erase;

	number_of_pages_to_erase = ((size_bytes + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE);

	for (uint32_t page_index = 0; page_index < number_of_pages_to_erase; page_index++)
	{
		status = flash_erase_page(start_address + (page_index * FLASH_PAGE_SIZE));

		if (status != FLASH_OK)
		{
			flash_lock();
			return status;
		}
	}

	flash_lock();
	return FLASH_OK;
}


/*
 * Program Flash memory.
 */
flash_status_t flash_program(uint32_t flash_address,
		uint16_t *chunk_buffer, uint32_t halfword_count)
{
	flash_status_t status;
	status = flash_unlock();
	if (status != FLASH_OK)
	{
		flash_lock();
		println("Flash unlock failed");
		return status;
	}


	status = flash_program_buffer(flash_address, chunk_buffer, halfword_count);
	if (status != FLASH_OK)
	{
		flash_lock();
		println("Flash write failed");
		return status;
	}


	flash_lock();
	return FLASH_OK;
}




