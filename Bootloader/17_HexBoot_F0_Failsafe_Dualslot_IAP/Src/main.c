/******************************************************************************
 * Project 17 Bring-up Test
 *
 * Purpose:
 * Validate Metadata and Slot Manager modules in isolation before integrating
 * them into the full bootloader.
 ******************************************************************************/

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

int main(void)
{
	bootloader_init();

    println("Project 17 Test");


    printf("SlotA.valid = %d\r\n", metadata.slot_a.is_valid);
    printf("SlotA.version = %lu\r\n", metadata.slot_a.version);
    printf("SlotA.size = %lu\r\n", metadata.slot_a.size);
    printf("SlotA.crc = %lu\r\n", metadata.slot_a.crc);

    println("Metadata Initialized");

    printf("Active Slot : %d\r\n",
            metadata_get_active_slot(&metadata));

    printf("Selected Slot : %d\r\n",
            slot_manager_select_boot_slot(&metadata));

    while(1);
}
