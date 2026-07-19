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

static firmware_info_t firmware_get_info(slot_t slot);
static uint16_t firmware_verify_crc(slot_t target_slot);
static void bootloader_send_requested_slot(slot_t slot);
void flash_handle_status(flash_status_t status);
uint8_t receive_firmware(slot_t target_slot);
static uint16_t chunk_index = 0;
static uint16_t expected_crc = 0;
static uint16_t payload_length = 0;




void bootloader_init(void)
{
    debug_uart_init(19200);

    dma_init();

    firmware_pingpong_init(&fw_pingpong);

    metadata_init(&metadata);

    firmware_receiver_init();
}



void bootloader_run(void)
{
	if(get_rx_update())
	{
		firmware_receiver_dma_callback(0);

		if(firmware_rx_get_state() == PARSE_HEADER)
		{
			firmware_rx_process();	//Validate the received firmware header and extract

			slot_t target_slot;

			target_slot = slot_manager_get_inactive_slot(&metadata);

			firmware_info_t fw = firmware_get_info(target_slot);

			payload_length = fw.payload_length;
			expected_crc = fw.crc;
			firmware_set_payload_info(fw.payload_length, fw.crc);

			bootloader_send_requested_slot(target_slot);

			if(receive_firmware(target_slot)){
				printf("SlotA.valid = %d\r\n", metadata.slot_a.is_valid);
				printf("SlotA.version = %lu\r\n", metadata.slot_a.version);
				printf("SlotA.size = %lu\r\n", metadata.slot_a.size);
				printf("SlotA.crc = 0x%08lX\r\n", metadata.slot_a.crc);
				println("Metadata Initialized");
				printf("Active Slot : %d\r\n", metadata_get_active_slot(&metadata));
				printf("Selected Slot : %d\r\n", slot_manager_select_boot_slot(&metadata));
				chunk_index = 0;

				jump();
			}

		}

	}
}



uint8_t receive_firmware(slot_t target_slot){
	/* Flash */
	flash_status_t flash_status;

	/* State */
	static uint8_t firmware_verified = 0;

	/* CRC */
	uint16_t calculated_crc = 0;


	if(header_received)
	{
		header_received = 0;
		firmware_rx_process(); /* Arm DMA immediately for payload reception */
		printf("payload length : %d\r\n",  payload_length);
		flash_status = flash_erase(slot_manager_get_slot_address(target_slot), payload_length);
		flash_handle_status(flash_status);
	}


	/* Flash Programming */
	if(fw_pingpong.prog_ready)
	{
		println("prog_ready");
		fw_pingpong.prog_ready = 0;
		uint32_t flash_address = (slot_manager_get_slot_address(target_slot) + (chunk_index * CHUNK_SIZE));

		/* Program chunk */
		flash_status = flash_program( flash_address,(uint16_t*)fw_pingpong.prog_buffer , ((program_chunk_size + 1)/2));
		flash_handle_status(flash_status);
		chunk_index++;
	}


	/* Firmware Verification */
	if(firmware_rx_get_state() == FW_COMPLETE && (!firmware_verified))
	{
		println("firmare_verified");
		calculated_crc = firmware_verify_crc(target_slot);
		firmware_verified = 1;
	}


	/* Boot Application */
	if(firmware_verified){

		/*----------    WRITING META DATA    ---------*/
		slot_info_t *slot_info = slot_manager_get_slot_info(&metadata, target_slot);
		slot_info->confirmed = 1;
		slot_info->crc = calculated_crc;
		slot_info->is_valid = 1;
		slot_info->size = payload_length;
		slot_info->version ++ ;
		metadata.active_slot = target_slot;
		metadata.boot_state = BOOT_NORMAL;

		metadata_save(&metadata);
		firmware_verified = 0;
		return 1;
	}
	return 0;

}


uint16_t firmware_verify_crc(slot_t target_slot){
	/*
	 * Verify programmed firmware against the CRC
	 * received in the packet header.
	 */
	uint16_t calculated_crc;
	calculated_crc = crc16_calculate((uint8_t*)slot_manager_get_slot_address(target_slot), payload_length);
	printf("Calculated CRC : 0x%04X\r\n", calculated_crc);

	if (calculated_crc == expected_crc) {
	println("CRC verified successfully");
	}
	return calculated_crc;
}


static void bootloader_send_requested_slot(slot_t slot){
    if(slot == SLOT_A)
    {
        println("Send Application : A");
    }
    else
    {
        println("Send Application : B");
    }
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


static firmware_info_t firmware_get_info(slot_t slot)
{
    firmware_info_t info;

    switch(slot)
    			{
    			case SLOT_A:
    				info.payload_length = payload_length_A;
    				info.crc = expected_crc_A;
    			    break;

    			case SLOT_B:
    				info.payload_length = payload_length_B;
    				info.crc = expected_crc_B;
    			    break;

    			default:
    			    info.payload_length = 0;
    			    info.crc = 0;
    			    break;
    			}

    return info;
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

		println("jump fail");
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


void jump(void){
    slot_t boot_slot = slot_manager_select_boot_slot(&metadata);

    printf("Boot Slot : %d\r\n", boot_slot);

    if (boot_slot == SLOT_NONE)
    {
        println("No bootable application.");
        return;
    }

    uint32_t app_address = slot_manager_get_slot_address(boot_slot);

    printf("App Address : 0x%08lX\r\n", (unsigned long)app_address);

    printf("MSP : 0x%08lX\r\n", *(uint32_t *)app_address);
    printf("Reset : 0x%08lX\r\n", *(uint32_t *)(app_address + 4));


    jmp_to_app(app_address);
}
