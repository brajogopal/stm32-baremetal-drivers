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



#define CHUNK_SIZE 128
#define FW_HEADER 0xAA


volatile uint8_t rx_buffer[CHUNK_SIZE];
volatile uint8_t chunk_ready = 0;
volatile uint16_t count = 0;
volatile uint16_t rx_index = 0;

volatile uint32_t bytes_received = 0;
volatile uint16_t payload_length_bytes = 0;

int main() {
	debug_uart_init(9600);
	flash_status_t flash_status;
	uint8_t header;
	uint16_t received_crc;
	uint16_t calculated_crc;


	println("Ready to receive Firmware");
		/*
		 Packet Format
		 Byte 0      : Header (FW_HEADER)
		 Byte 1-2    : Payload Length (bytes)
		 Byte 1-2    : CRC16
		 Byte N      : Payload
		 */

		uart_status = uart_receive_with_timeout(&header, UART_TIMEOUT_LONG);
		if (uart_status != UART_OK) {
			println("UART Timeout");
			jmp_to_app(APPLICATION_A_ADDRESS);
			while (1);
		} else {
			println("Firmware Header Receive successful");
		}



		if (header != FW_HEADER) {
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


		uart_status = reconstruct_halfword_timeout(&received_crc, UART_TIMEOUT_LONG);

		if (uart_status != UART_OK) {
			println("CRC Receive Failed");
			while (1);
		} else {
			printf("Received CRC : 0x%04X\r\n", received_crc);
		}




	/***************************************************************/
	/* From here the uart interrupt and payload receive code start */
	/***************************************************************/

	uart_interrupt_init(9600);

	while (1) {
		volatile uint32_t total = 0;
		if(chunk_ready){

			__disable_irq();

			count++;
			chunk_ready = 0;
			uint16_t len = rx_index;
			rx_index = 0;
			total = bytes_received;



			if(count == 1){
				/*------- Erasing memory region-------*/
				erase_flash_region(APPLICATION_A_ADDRESS, payload_length_bytes);
			}


			uint32_t flash_address = (APPLICATION_A_ADDRESS + (count - 1) * CHUNK_SIZE);

			/* Program chunk */
			flash_status = program_flash_chunk( flash_address,(uint16_t*)rx_buffer, ((len + 1)/2));
			flash_handle_status(flash_status);


			__enable_irq();
		}




		/**********************************************/
		/*------- If Firmware Receive Complete -------*/
		/**********************************************/
		if(total >= payload_length_bytes){
			USART2->CR1 &= ~UART_RXNEIE;


			println("Firmware Receive successful");

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
			 bytes_received = 0;
		}
	}
}


void USART2_IRQHandler(void){
	if(USART2->ISR & UART_RXNE)
	{
		uint8_t data = USART2->RDR;

		if(rx_index < CHUNK_SIZE)
		{
			rx_buffer[rx_index++] = data;
			bytes_received++;
			if(rx_index >= CHUNK_SIZE || bytes_received >= payload_length_bytes)
			{
				chunk_ready = 1;
			}
		}
	}
}
