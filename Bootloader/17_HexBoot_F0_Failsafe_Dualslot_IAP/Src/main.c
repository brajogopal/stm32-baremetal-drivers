/******************************************************************************
 * Project 17 Bootloader Entry Point
 ******************************************************************************/



#include "bootloader.h"
#include "uart.h"


int main(void)
{

	bootloader_init();
	println("bootloader init");
    while(1){

        bootloader_run();


    }
}
