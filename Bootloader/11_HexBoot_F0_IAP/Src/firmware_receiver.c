/*
 * firmware_receiver.c
 *
 *  Created on: 28-May-2026
 *      Author: brajo
 */

#include "uart.h"
#include "firmware_receiver.h"
#include "flash_driver.h"
#include <stdio.h>

#define FLASH_PAGE_SIZE		0x400U	// 1KB


uint16_t reconstruct_halfword(void)
{
    uint8_t low;
    uint8_t high;


    low  = uart_receive_char();
    high = uart_receive_char();

    return (low | (high << 8));
}

uart_status_t reconstruct_halfword_timeout(uint16_t *data,uint32_t timeout)
{
    uint8_t low;
    uint8_t high;
    uart_status_t status;


    status = uart_receive_with_timeout(&low, timeout);
    if(status != UART_OK){
    	return status;
    	}



    status = uart_receive_with_timeout(&high, timeout);
    if(status != UART_OK){
       	return status;
       }


    *data = (low | (high << 8));
    return UART_OK;
}




uart_status_t firmware_receive_chunk(uint32_t addr,uint16_t *data, uint32_t length, uint32_t timeout){
	/* uint32_t addr ---  Reserved for future dynamic chunk storage */

	for(int i=0; i<length; i++){
		uart_status_t status;
		status = reconstruct_halfword_timeout(&data[i], timeout);
		if(status != UART_OK)
		{
		    println("RX Timeout");
		    return status;
		}
	}
	return UART_OK;
}




uart_status_t firmware_receive_8bit(uint32_t addr,uint8_t *data, uint32_t length, uint32_t timeout){
	/* uint32_t addr ---  Reserved for future dynamic chunk storage */
	for(int i = 0; i < length; i++)
	{
		uart_status_t status;
	    status = uart_receive_with_timeout(&data[i], timeout);
	    if(status != UART_OK)
	    	{
	    	    println("RX Timeout");
	    	    return status;
	   		}
	}
	return UART_OK;

}





flash_status_t store_data_in_flash(uint32_t addr,uint16_t *data, uint32_t length){
	flash_status_t status;
	status = flash_unlock();
	if(status != FLASH_OK){
		flash_lock();
		println("Flash unlocked failed");
		return status;
		}



	/*-------------- Dynamic Flash Erased -------------*/
	uint32_t firmware_size_bytes, total_flash_space_required, number_of_pages_to_erase;

    firmware_size_bytes = length * 2;

    total_flash_space_required = firmware_size_bytes + (FLASH_PAGE_SIZE - 1);

	number_of_pages_to_erase = total_flash_space_required / FLASH_PAGE_SIZE;

	    for(uint32_t page_index = 0; page_index < number_of_pages_to_erase; page_index++)
	    {
	        status = flash_erase_page(addr + (page_index * FLASH_PAGE_SIZE));

	        if(status != FLASH_OK)
	        {
	        	flash_lock();
	            return status;
	        }
	    }





	status = flash_program_buffer(addr, data, length);
	if(status != FLASH_OK){
		flash_lock();
		println("Flash Write Failed");
		return status;
		}


	flash_lock();
	return FLASH_OK;
}
