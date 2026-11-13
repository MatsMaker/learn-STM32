/* Rendering helpers (ported from doom-nano for STM32 + stm32-ssd1306 lib) */
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include "constants.h"

/* Frame buffer, same layout as SSD1306 GDDRAM: byte = 8 vertical pixels */
extern uint8_t display_buf[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
extern uint8_t zbuffer[ZBUFFER_SIZE];
extern float delta;

void setupDisplay(void);
void display_update(void);                 /* push frame buffer to the OLED */
void display_invert(bool invert);          /* hardware invert */
void display_clearRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void display_drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, bool color);

void fps(void);
float getActualFps(void);
bool getGradientPixel(uint8_t x, uint8_t y, uint8_t i);
void fadeScreen(uint8_t intensity, bool color);
void drawByte(uint8_t x, uint8_t y, uint8_t b);
void drawPixel(int16_t x, int16_t y, bool color, bool raycasterViewport);
void drawVLine(uint8_t x, int8_t start_y, int8_t end_y, uint8_t intensity);
void drawSprite(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[],
                int16_t w, int16_t h, uint8_t sprite, float distance);
void drawChar(int8_t x, int8_t y, char ch);
void drawText(int8_t x, int8_t y, const char *txt, uint8_t space);
void drawNumber(uint8_t x, uint8_t y, uint8_t num);

#endif /* DISPLAY_H */
