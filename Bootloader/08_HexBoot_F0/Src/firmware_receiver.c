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




flash_status_t firmware_receive_chunk(uint32_t addr,uint16_t *data, uint32_t length, uint32_t timeout){
	/* uint32_t addr ---  Reserved for future dynamic chunk storage */

	for(int i=0; i<length; i++){
		uart_status_t status;
		status = reconstruct_halfword_timeout(&data[i], timeout);
		//data[i] = reconstruct_halfword();
		if(status == UART_TIMEOUT)
		{
		    println("RX Timeout");
		    return FLASH_ERROR;
		}
		else
		{
		printf("[%02d] = ",i);
		printf("0x%04X\r\n", data[i]);	//Echo character
		}
	}
	return FLASH_OK;
}





flash_status_t store_data_in_flash(uint32_t addr,uint16_t *data, uint32_t length){
	flash_status_t status;
	status = flash_unlock();
	if(status != FLASH_OK){
			flash_lock();
			return status;
		}


	status = flash_erase_page(addr);
	if(status != FLASH_OK){
			flash_lock();
			return status;
		}


	status = flash_program_buffer(addr, data, length);
	if(status != FLASH_OK){
				flash_lock();
				return status;
			}


	flash_lock();
	return FLASH_OK;
}
