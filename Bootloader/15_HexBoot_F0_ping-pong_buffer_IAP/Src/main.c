#include "stm32f030x8.h"
#include "delay.h"
#include "uart.h"
#include "bsp.h"
#include "bootloader.h"
#include "flash_driver.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"
#include "crc.h"
#include <stdio.h>


volatile uint16_t count = 0;
volatile uint32_t total = 0;
volatile uint32_t overrun_count = 0;



void verify_crc(void);

int main() {
	static uint8_t verified = 0;
	static uint32_t last_ore = 0;
	static volatile uint8_t flash_write_complete = 0;
	flash_status_t flash_status;
	firmware_pingpong_init(&fw_pingpong);
	uart_interrupt_init(19200);

	println("Ready to receive Firmware");
		/*
		 Packet Format
		 Byte 0      : Header (FW_HEADER)
		 Byte 1-2    : Payload Length (bytes)
		 Byte 1-2    : CRC16
		 Byte N      : Payload
		 */

	while (1) {


		if(erase_whole){
			erase_whole = 0;
			erase_flash_region(APPLICATION_A_ADDRESS,payload_length);
		}

		/*
		 * Ping-pong buffering allows reception and flash
		 * programming to operate independently.
		 *
		 * Remaining limitation:
		 * UART is still interrupt-per-byte, which can
		 * generate ORE (Overrun Error) at high transfer rates.
		 * DMA reception will be explored in Project 16.
		 */

		if(fw_pingpong.prog_ready){

			__disable_irq();

			if(overrun_count != last_ore)
			{
			    printf("ORE : %lu\r\n", overrun_count);
			    last_ore = overrun_count;
			}

			flash_write_complete = 0;
			fw_pingpong.prog_ready = 0;
			count++;

			__enable_irq();

			uint32_t flash_address = (APPLICATION_A_ADDRESS + (count - 1) * CHUNK_SIZE);

			/* Program chunk */
			flash_status = program_flash_chunk( flash_address,(uint16_t*)fw_pingpong.prog_buffer , ((len + 1)/2));
			flash_handle_status(flash_status);


			if(last_chunk){
			flash_write_complete = 1;
			}

		}


		if(firmware_rx_get_state() == FW_COMPLETE && (!verified))
		{
			if(flash_write_complete )
			{
			    verify_crc();
			    verified = 1;
			}

		}
	}
}


void USART2_IRQHandler(void)
{
	 /* Check Overrun Error */
	    if(USART2->ISR & USART_ISR_ORE)
	    {
	    	 overrun_count++;

	        /* Clear ORE */
	        USART2->ICR |= USART_ICR_ORECF;
	    }


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

