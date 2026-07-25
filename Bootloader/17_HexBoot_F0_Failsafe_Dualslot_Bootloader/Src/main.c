/******************************************************************************
 * Project 17 Bootloader Entry Point
 ******************************************************************************/


#include "bootloader.h"


int main(void)
{
	bootloader_init();

    while(1)
    {
        bootloader_run();
    }
}
