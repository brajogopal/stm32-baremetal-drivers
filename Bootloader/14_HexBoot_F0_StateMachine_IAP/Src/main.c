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


volatile uint16_t count = 0;
volatile uint32_t total = 0;


void verify_crc(void);

int main() {
	static uint8_t verified = 0;
	static volatile uint8_t flash_write_complete = 0;
	flash_status_t flash_status;
	uart_interrupt_init(9600);
	println("Ready to receive Firmware");
		/*
		 Packet Format
		 Byte 0      : Header (FW_HEADER)
		 Byte 1-2    : Payload Length (bytes)
		 Byte 1-2    : CRC16
		 Byte N      : Payload
		 */

	while (1) {
		flash_write_complete = 0;
		if(erase_whole){
			erase_whole = 0;
			erase_flash_region(APPLICATION_A_ADDRESS,payload_length);
		}



		/*
		 * UART interrupts are temporarily disabled because
		 * Project 14 uses a single receive buffer.
		 *
		 * Future Project 15:
		 * Replace with ping-pong buffering to allow
		 * reception during flash programming.
		 */
		if(chunk_ready){
			__disable_irq();
			chunk_ready = 0;
			count++;
			uint32_t flash_address = (APPLICATION_A_ADDRESS + (count - 1) * CHUNK_SIZE);

			/* Program chunk */
			flash_status = program_flash_chunk( flash_address,(uint16_t*)rx_buffer, ((len + 1)/2));
			flash_handle_status(flash_status);
			 flash_write_complete = 1;
			__enable_irq();
		}


		if(firmware_rx_get_state() == FW_COMPLETE && (!verified))
		{
			if(flash_write_complete)
			{
			    verify_crc();
			    verified = 1;
			}

		}
	}
}


void USART2_IRQHandler(void)
{
    if(USART2->ISR & UART_RXNE)
    {
        uint8_t data = USART2->RDR;

        firmware_rx_process_byte(data);
    }
}

void verify_crc(void){
	/*
	 * Verify programmed firmware against the CRC
	 * received in the packet header.
	 */
	uint16_t calculated_crc;
	calculated_crc = crc16_calculate((uint8_t*)APPLICATION_A_ADDRESS, payload_length);
	printf("Calculated CRC : 0x%04X\r\n", calculated_crc);

	if (calculated_crc == expected_crc) {
	println("CRC verified successfully");
	}
}

