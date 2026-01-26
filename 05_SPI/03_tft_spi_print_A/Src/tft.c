/*
 * tft.c
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */


#include "tft.h"
#include "spi.h"
#include "board_spi.h"
#include "delay.h"
#include "stm32f030x8.h"



#define 	TFT_RST		(1U<<2)//TFT RST pin to PA2
#define 	TFT_A0		(1U<<3)//TFT A0(D/CX) pin to PA3
#define 	TFT_WIDTH   128
#define 	TFT_HEIGHT  160

static void tft_begin(void)  { board_cs_enable(); }
static void tft_end(void)    { board_cs_disable(); }
static inline void tft_dc_cmd(void)  { GPIOA->ODR &= ~TFT_A0; }//TFT A0(D/CX) pin LOW
static inline void tft_dc_data(void) { GPIOA->ODR |=  TFT_A0; }//TFT A0(D/CX) pin HIGH

static const uint8_t glyph_A[5] = {
    0b1111110,
    0b0001001,
    0b0001001,
    0b0001001,
    0b1111110
};


void tft_gpio_init(void)
{
    RCC->AHBENR |= (1U << 17); // GPIOA Enable

    // PA3 = A0 (output)
    GPIOA->MODER &= ~(3U << 6);
    GPIOA->MODER |=  (1U << 6);

    // PA2 = RST (output)
    GPIOA->MODER &= ~(3U << 4);
    GPIOA->MODER |=  (1U << 4);
}

void tft_write_cmd(uint8_t cmd)
{

	tft_dc_cmd();
    spi1_transmit(&cmd, 1);

}

void tft_write_data(uint8_t data)
{

	tft_dc_data();
    spi1_transmit(&data, 1);

}

void tft_reset(void)
{
    GPIOA->ODR &= ~TFT_RST;//TFT_RST LOW
    delay_ms(20);

    GPIOA->ODR |=  TFT_RST;//TFT_RST HIGH
    delay_ms(120);
}

void tft_init(void)
{
	tft_begin();

    tft_gpio_init();
    tft_reset();

    tft_write_cmd(0x01); // SWRESET

    tft_end();
    delay_ms(150);		//  Give time to internal voltage stabilize
    tft_begin();

    tft_write_cmd(0x11); // SLPOUT

    tft_end();
    delay_ms(150);		//  Give time to internal voltage stabilize
    tft_begin();

	tft_write_cmd(0x3A);    //COLMOD
	tft_write_data(0x05);   //RGB565

	// Display is configured with MV=1 (X/Y swapped)
	// All pixel streaming assumes column-major font + MV addressing
	tft_write_cmd(0x36);      // MADCTL
	tft_write_data(0xA8);

    tft_write_cmd(0x29); // DISPON

    tft_end();
}

static void tft_set_window(uint16_t x0, uint16_t y0,
                           uint16_t x1, uint16_t y1)
{

    tft_write_cmd(0x2A);		 // CASET
    tft_write_data(x0 >> 8);
    tft_write_data(x0 & 0xFF);
    tft_write_data(x1 >> 8);
    tft_write_data(x1 & 0xFF);

    tft_write_cmd(0x2B);		// RASET
    tft_write_data(y0 >> 8);
    tft_write_data(y0 & 0xFF);
    tft_write_data(y1 >> 8);
    tft_write_data(y1 & 0xFF);
}



void tft_fill_screen(uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;



    tft_begin();

    // Set full screen window
    tft_set_window(0, 0, (TFT_HEIGHT-1), (TFT_WIDTH-1));

    // Write pixel data
    tft_write_cmd(0x2C);          // RAMWR

    for (int i = 0; i < TFT_WIDTH * TFT_HEIGHT; i++) {
        tft_write_data(hi);
        tft_write_data(lo);
    }

    tft_end();
}

void tft_half_fill_screen(uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    tft_begin();

    // Set 1/4 screen window
    tft_set_window(0, 0, 60, 80);

    // Write pixel data
    tft_write_cmd(0x2C);          // RAMWR

    for (int i = 0; i < 61 * 81; i++) {
        tft_write_data(hi);
        tft_write_data(lo);
    }

    tft_end();
}

void tft_draw_A(uint16_t x, uint16_t y,
                uint16_t fg, uint16_t bg)
{
    uint8_t row, col;
    uint8_t fg_hi = fg >> 8;
    uint8_t fg_lo = fg & 0xFF;
    uint8_t bg_hi = bg >> 8;
    uint8_t bg_lo = bg & 0xFF;

    tft_begin();

    // Define 5x7 window
    tft_set_window(x, y, x + 4, y + 6);

    // Start memory write
    tft_write_cmd(0x2C);   // RAMWR

    // Stream pixels
    for (row = 0; row < 7; row++) {
        for (col = 0; col < 5; col++) {

            if ((glyph_A[col] >> row) & 1) {
                tft_write_data(fg_hi);
                tft_write_data(fg_lo);
            } else {
                tft_write_data(bg_hi);
                tft_write_data(bg_lo);
            }
        }
    }

    tft_end();
}


