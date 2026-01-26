#include 	"spi.h"
#include 	"board_spi.h"
#include	"tft.h"
#include	"delay.h"



int main(void){

	board_spi1_gpio_init();
	spi1_init();

    tft_init();

    tft_fill(0xFFFF);           // WHITE
    tft_set_cursor(10,15);
    text_color(0x000F); //(Text Color)
    tft_print("HELLO WORLD STM32", 1);//(Text,Size)

    while(1){}

}
