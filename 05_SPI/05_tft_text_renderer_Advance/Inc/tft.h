/*
 * tft.h
 *
 *  Created on: 21-Jan-2026
 *      Author: brajo
 */

#ifndef TFT_H_
#define TFT_H_
#include "stdint.h"


void tft_init(void);
void tft_fill_screen(uint16_t color);
void tft_draw_char(uint16_t x, uint16_t y, char c,uint16_t fg, uint16_t bg, uint8_t size);
void tft_set_cursor(uint16_t x, uint16_t y);
void tft_fill(uint16_t color);
void text_color(uint16_t fg);
void tft_set_text_color(uint16_t fg, uint16_t bg);
void tft_print_char(char c,uint8_t size);
void tft_print(const char *str, uint8_t size);

#endif /* TFT_H_ */
