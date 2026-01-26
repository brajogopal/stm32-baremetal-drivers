#include 	"spi.h"
#include 	"board_spi.h"
#include	"tft.h"
#include	"delay.h"



int main(void){

	board_spi1_gpio_init();
	spi1_init();

    tft_init();
    tft_fill_screen(0x0000);           // black
    tft_draw_string(10, 10, "ABBA", 0xFFFF, 0x0000,2); // white A


    while(1){}
}
