#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "firmware_receiver.h"
#include <stdio.h>

#define TEST_STORAGE_ADDR   0x0800F800
#define UART_TIMEOUT_LONG 10000000UL
#define MAX_HALFWORDS 64



int main() {
	debug_uart_init(9600);

	flash_status_t flash_status;
	uart_status_t uart_status;
	uint8_t header;
	uint16_t payload_length_bytes;
	uint16_t halfword_count;
	uint16_t firmware_buffer[MAX_HALFWORDS];

	println("Ready to receive Firmware");
	/*
	Packet Format

	Byte 0      : Header (0xAA)
	Byte 1-2    : Payload Length (bytes)
	Byte N      : Payload
	*/



	uart_status = uart_receive_with_timeout(&header, UART_TIMEOUT_LONG);
	if (uart_status != UART_OK) {
		println("Firmware Header Receive Failed");
		while (1);
	} else {
		println("Firmware Header Receive successful");
	}

	if (header != 0xAA) {
		println("Invalid Header");
		while (1);
	} else {
		println("Header Matched");
	}




	uart_status = reconstruct_halfword_timeout(&payload_length_bytes, UART_TIMEOUT_LONG);

	if (uart_status != UART_OK) {
		println("Firmware length Receive Failed");
		while (1);
	} else {
		printf("Firmware length Receive successful : %u\r\n", payload_length_bytes);
	}


	halfword_count = payload_length_bytes / 2;
	if ((payload_length_bytes % 2 != 0) || (halfword_count >  MAX_HALFWORDS)) {
		println("Invalid Payload Length");
		while (1);
	}




	flash_status = firmware_receive_chunk(TEST_STORAGE_ADDR, firmware_buffer,
			halfword_count, UART_TIMEOUT_LONG);

	if (flash_status != FLASH_OK) {
		println("Firmware Receive Failed");
	} else {
		println("Firmware Receive successful");
	}

	flash_status = store_data_in_flash(TEST_STORAGE_ADDR, firmware_buffer,
			halfword_count);

	if (flash_status != FLASH_OK) {
		println("Firmware Update Failed");
	} else {
		println("Firmware Update successful");
	}





	while (1) {
	}

}
