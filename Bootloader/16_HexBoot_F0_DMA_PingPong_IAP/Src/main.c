/*
 * main.c
 *
 * Project : HexBoot_F0_DMA_PingPong_IAP
 *
 * Description:
 * Application entry point.
 *
 * Initializes the bootloader modules and executes the
 * event-driven firmware update loop.
 */


/* MCU */
#include "stm32f030x8.h"

/* Drivers */
#include "uart.h"
#include "flash_driver.h"
#include "crc.h"
#include "dma.h"

/* Bootloader */
#include "bootloader.h"
#include "metadata.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"

/* BSP */
#include "delay.h"
#include "bsp.h"


/*Global State */
volatile uint16_t chunk_index = 0;
volatile uint8_t rx_data_ready = 0;


uint16_t firmware_verify_crc(void);


int main() {
	/* Flash */
	flash_status_t flash_status;

	/* CRC */
	uint16_t calculated_crc;

	/* State */
	static uint8_t firmware_verified = 0;


	firmware_pingpong_init(&fw_pingpong);
	debug_uart_init(19200);
	dma_init();
	firmware_rx_process();


	println("Ready to receive Firmware");

		/*
		 Packet Format
		 Byte 0      : Header (FW_HEADER)
		 Byte 1-2    : Payload Length (bytes)
		 Byte 1-2    : CRC16
		 Byte N      : Payload
		 */


	while (1) {

		/* Receive Events */
		if(rx_data_ready)
		{
			rx_data_ready = 0;
		    firmware_rx_process();
		}



		if(header_received)
		{
			header_received = 0;
			firmware_rx_process();  // Immediately arm DMA to receive firmware
			flash_status = erase_flash_region(APPLICATION_A_ADDRESS, payload_length);
			flash_handle_status(flash_status);
		}


		/* Flash Programming */
		if(fw_pingpong.prog_ready)
		{
		fw_pingpong.prog_ready = 0;
		uint32_t flash_address = (APPLICATION_A_ADDRESS + (chunk_index * CHUNK_SIZE));

		/* Program chunk */
		flash_status = program_flash_chunk( flash_address,(uint16_t*)fw_pingpong.prog_buffer , ((program_chunk_size + 1)/2));
		flash_handle_status(flash_status);
		chunk_index++;
		}


		/* Firmware Verification */
		if(firmware_rx_get_state() == FW_COMPLETE && (!firmware_verified))
		{
			    calculated_crc = firmware_verify_crc();
			    firmware_verified = 1;
		}


		/* Boot Application */
		if(firmware_verified){
			/*----------    WRITING META DATA    ---------*/
			if (flash_status == FLASH_OK) {

				firmware_metadata_t metadata;

				metadata.magic_number = APP_MAGIC;
				metadata.firmware_length = payload_length;
				metadata.firmware_crc = calculated_crc;

				flash_status = metadata_write(METADATA_SLOT_A,&metadata);
				flash_handle_status(flash_status);
			}
			 jmp_to_app(APPLICATION_A_ADDRESS);
		}
	}
}



void DMA1_CH4_5_IRQHandler(void){
    if(DMA1->ISR & DMA_ISR_TCIF5)	//Read
    {
    	rx_data_ready = 1;
        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
    }

    if(DMA1->ISR & DMA_ISR_TEIF5)	//Read
    {
        println("DMA ERROR");

        DMA1->IFCR |= DMA_IFCR_CGIF5;//Clear
    }
}


uint16_t firmware_verify_crc(void){
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
	return calculated_crc;
}



