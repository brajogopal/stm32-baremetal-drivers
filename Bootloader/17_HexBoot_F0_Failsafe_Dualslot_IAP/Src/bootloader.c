/*
 * bootloader.c
 *
 * Project : HexBoot_F0_DMA_PingPong_IAP
 *
 * Description:
 * Implements the bootloader startup sequence.
 *
 * Responsibilities:
 * - Validate application metadata.
 * - Verify firmware integrity.
 * - Configure application stack pointer.
 * - Jump to the user application.
 *
 *  Created on: 26-Apr-2026
 *      Author: brajo
 */

/* MCU */
#include <flash.h>
#include "stm32f030x8.h"

/* Drivers */
#include "uart.h"
#include "crc.h"
#include "dma.h"

/* Bootloader */
#include "bootloader.h"
#include "metadata.h"
#include "slot_manager.h"
#include "firmware_receiver.h"
#include "firmware_pingpong.h"

/* BSP */
#include "delay.h"
#include "bsp.h"

#include <stdio.h>



#define SYSCFG_EN (1U<<0)


static uint16_t firmware_verify_crc(void);
void flash_handle_status(flash_status_t status);
void receive_firmware(void);
static uint16_t chunk_index = 0;
static slot_t slot;




void bootloader_init(void)
{
    debug_uart_init(19200);

    dma_init();

    firmware_pingpong_init(&fw_pingpong);

    metadata_init(&metadata);

    firmware_receiver_init();
}



void bootloader_run(void){

	if(get_rx_update())
	{
		firmware_receiver_dma_callback(0);
		firmware_rx_process();
		println("rx received");
	}

	receive_firmware();

}

void receive_firmware(void){
	/* Flash */
	flash_status_t flash_status;

	/* CRC */
	uint16_t calculated_crc;

	/* State */
	static uint8_t firmware_verified = 0;



	if(header_received)
	{
		header_received = 0;
		firmware_rx_process();  // Immediately arm DMA to receive firmware
		slot = slot_manager_get_inactive_slot(&metadata);
		flash_status = flash_erase(slot_manager_get_slot_address(slot), payload_length);
		flash_handle_status(flash_status);
	}


	/* Flash Programming */
	if(fw_pingpong.prog_ready)
	{
		println("prog_ready");
	fw_pingpong.prog_ready = 0;
	uint32_t flash_address = (slot_manager_get_slot_address(slot) + (chunk_index * CHUNK_SIZE));

	/* Program chunk */
	flash_status = flash_program( flash_address,(uint16_t*)fw_pingpong.prog_buffer , ((program_chunk_size + 1)/2));
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
		printf("CRC = %04X", calculated_crc);
		while(1);
	}

}


uint16_t firmware_verify_crc(void){
	/*
	 * Verify programmed firmware against the CRC
	 * received in the packet header.
	 */
	uint16_t calculated_crc;
	calculated_crc = crc16_calculate((uint8_t*)slot_manager_get_slot_address(slot), payload_length);
	printf("Calculated CRC : 0x%04X\r\n", calculated_crc);

	if (calculated_crc == expected_crc) {
	println("CRC verified successfully");
	}
	return calculated_crc;
}


void flash_handle_status(flash_status_t status)
{
	switch(status)
	{
	case FLASH_OK:
		println("FLASH_OK");
		break;

	case FLASH_ERROR:
		//flash_lock();
		println("FLASH_ERROR");
		break;

	case FLASH_TIMEOUT:
		//flash_lock();
		println("FLASH_TIMEOUT");
		break;

	default:
		println("UNKNOWN_FLASH_STATE");
	}
}





void jmp_to_app(uint32_t app_address) {
	func_ptr jump_to_app;
	println("bootloader started..");
	delay_ms(300);

	if (is_valid_app(app_address)) {

		__disable_irq();
		relocate_vector_table(app_address);

		/*Initialize main stack pointer */
		__set_MSP(*(uint32_t*) app_address);

		uint32_t app_start_address = *(uint32_t*) (app_address + 4);
		jump_to_app = (func_ptr) (app_start_address);
		println("valid application present..");

		/*jump*/
		jump_to_app();

		while (1);//Fail-safe design(If execution ever comes back, CPU gets stuck here safely)
	} else {
		println("No valid application found");
	}
}

int is_valid_app(uint32_t addr) {
	uint32_t msp = *(uint32_t*) addr;
	uint32_t reset = *(uint32_t*) (addr + 4);

	// MSP must be in SRAM
	if (msp < SRAM_START || msp > SRAM_END){
		return 0;
	}

	// MSP must be aligned to 32 bit(4 byte)
	if (msp % 4){
		return 0;
	}
	// Reset handler must be in Flash
	if (reset < FLASH_START || reset > FLASH_END){
		return 0;
	}
	// Thumb bit check(Cortex-M supports only Thumb mode, Function address must have LSB=1)
	if ((reset & 1) == 0){
		return 0;
	}

	return 1;
}



void relocate_vector_table(uint32_t app_address) {
	uint32_t *src = (uint32_t*) app_address;
	uint32_t *dst = (uint32_t*) SRAM_START;

	for (uint32_t i = 0; i < VECTOR_COUNT; i++) {   // copying of vectors
		dst[i] = src[i];
	}

	RCC->APB2ENR |= SYSCFG_EN;
	SYSCFG->CFGR1 &= ~(3 << 0);
	SYSCFG->CFGR1 |= (3 << 0);   // SRAM mapped at 0x00000000

	__DSB();   // Data Synchronization Barrier
	__ISB();   // Instruction Synchronization Barrier
}

