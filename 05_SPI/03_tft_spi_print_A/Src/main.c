#include 	"spi.h"
#include 	"board_spi.h"
#include	"tft.h"
#include	"delay.h"



int main(void){

	board_spi1_gpio_init();
	spi1_init();

    tft_init();
    tft_fill_screen(0x0000);           // black
    tft_draw_A(10, 10, 0xFFFF, 0x0000); // white A
    tft_draw_A(20, 10, 0xFFFF, 0x0000); // white A
    tft_draw_A(30, 10, 0xFFFF, 0x0000); // white A
    tft_draw_A(40, 10, 0xFFFF, 0x0000); // white A

    while(1){}
}
