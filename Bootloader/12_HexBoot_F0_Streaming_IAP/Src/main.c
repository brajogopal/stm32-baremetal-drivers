#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include "crc.h"
#include <stdio.h>


#define UART_TIMEOUT_LONG 10000000UL
#define CHUNK_SIZE 128

int main() {
	debug_uart_init(9600);

	flash_status_t flash_status;
	uart_status_t uart_status;
	uint8_t header;
	uint16_t payload_length_bytes;
	uint16_t received_crc;
	uint16_t calculated_crc;

	println("Ready to receive Firmware");
	/*
	 Packet Format

	 Byte 0      : Header (0xAA)
	 Byte 1-2    : Payload Length (bytes)
	 Byte N      : Payload
	 Byte N+1-2  : CRC16
	 */

	uart_status = uart_receive_with_timeout(&header, UART_TIMEOUT_LONG);
	if (uart_status != UART_OK) {
		println("UART Timeout");
		jmp_to_app(APPLICATION_A_ADDRESS);
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
		printf("Firmware length Receive successful : %u byte\r\n", payload_length_bytes);
	}



	if ((payload_length_bytes % 2 != 0) || (payload_length_bytes > (24576 - 32))) {
		println("Invalid Payload Length");
		while (1);
	}




	uint16_t chunk_buffer[CHUNK_SIZE/2];
	uint32_t current_chunk_size;
	uint32_t remaining_bytes = payload_length_bytes;
	uint32_t write_count = 0;


	/*------- Erasing memory region-------*/
	flash_status = erase_flash_region(APPLICATION_A_ADDRESS, payload_length_bytes);
	if(flash_status != FLASH_OK)
	{
	    flash_handle_status(flash_status);
	    while(1);
	}
	else	{
		println("Flash erase done");
	}

	/*-------- Receiving Firmware --------*/
	while(remaining_bytes > 0)
	{
		uint16_t halfword_count;
		uint32_t flash_address = APPLICATION_A_ADDRESS + (write_count * CHUNK_SIZE);

		/*--- Debug script ---*/
		//printf("Chunk %lu\r\n", write_count);

		if(remaining_bytes > CHUNK_SIZE){
				current_chunk_size = CHUNK_SIZE;
				halfword_count = current_chunk_size / 2;


				/* Receive chunk */
				uart_status = firmware_receive_chunk(flash_address, chunk_buffer, halfword_count, UART_TIMEOUT_LONG);
				if (uart_status != UART_OK) {
					println("Firmware Receive Failed");
					while (1);
				}

				/* Program chunk */
				flash_status = program_flash_chunk( flash_address, chunk_buffer, halfword_count);
				if(flash_status != FLASH_OK)
				{
				    flash_handle_status(flash_status);
				    while(1);
				}
			}
			else	{
				current_chunk_size = remaining_bytes;
				halfword_count = current_chunk_size / 2;

				/* Receive chunk */
				uart_status = firmware_receive_chunk(flash_address, chunk_buffer, halfword_count, UART_TIMEOUT_LONG);
				if (uart_status != UART_OK) {
					println("Firmware Receive Failed");
					while (1);
				}

				/* Program chunk */
				flash_status = program_flash_chunk( flash_address, chunk_buffer, halfword_count);
				if(flash_status != FLASH_OK)
				{
				    flash_handle_status(flash_status);
				    while(1);
				}
		}
		write_count++;
		remaining_bytes -= current_chunk_size;
	}
	println("Firmware Receive successful");



	uart_status = reconstruct_halfword_timeout(&received_crc, UART_TIMEOUT_LONG);

	if (uart_status != UART_OK) {
		println("CRC Receive Failed");
		while (1);
	} else {
		printf("Received CRC : 0x%04X\r\n", received_crc);
	}


	calculated_crc = crc16_calculate((uint8_t*)APPLICATION_A_ADDRESS, payload_length_bytes);
	printf("Calculated CRC : 0x%04X\r\n", calculated_crc);


	if (calculated_crc == received_crc) {

		println("CRC verified successfully");

		/*----------    WRITING META DATA    ---------*/
		if (flash_status == FLASH_OK) {

			firmware_metadata_t metadata;

			metadata.magic_number = APP_MAGIC;
			metadata.firmware_length = payload_length_bytes;
			metadata.firmware_crc = calculated_crc;

			flash_status = metadata_write(METADATA_SLOT_A,&metadata);
			flash_handle_status(flash_status);
		}

		 jmp_to_app(APPLICATION_A_ADDRESS);
	}
	else {
		println("CRC verification failed");
		println("corrupted data");
		println("restart the process");
		while (1);
	}

	while (1) {
	}

}
