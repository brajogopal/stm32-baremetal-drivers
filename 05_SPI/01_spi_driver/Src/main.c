#include 	"spi.h"


int main(void){

    spi1_gpio_init();
    spi1_config();

    tft_init();
    while(1){

    }
}
