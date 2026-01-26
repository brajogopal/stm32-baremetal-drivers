#include 	"spi.h"
#include 	"board_spi.h"
#include 	<stdint.h>

uint8_t byte = 0xAA;

int main(void){

	board_spi1_gpio_init();
	spi1_init();

    board_cs_enable();
    spi1_transmit(&byte, 1);
    board_cs_disable();

    while(1){}
}
