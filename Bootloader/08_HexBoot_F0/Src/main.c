#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "storage_manager.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include <stdio.h>



#define TEST_STORAGE_ADDR   0x0800F800
uint16_t firmware_buffer[64];
uint32_t timeout = 10000;






int main() {
	debug_uart_init(9600);

	flash_status_t status;
	status = firmware_receive_chunk(TEST_STORAGE_ADDR, firmware_buffer, 64,timeout);

	if(status != FLASH_OK){
		println("Firmware Receive Failed");
	}	else	{
		println("Firmware Receive successful");
	}


	status = store_data_in_flash(TEST_STORAGE_ADDR, firmware_buffer, 64);

	if(status != FLASH_OK){
		println("Firmware Update Failed");
	}	else	{
		println("Firmware Update successful");
	}

	while (1) {
	}
}

