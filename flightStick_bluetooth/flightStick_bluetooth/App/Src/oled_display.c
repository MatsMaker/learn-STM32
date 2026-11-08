#include <stdio.h>

#include "oled_display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

void OLED_Display_Init(void)
{
    ssd1306_Init();
}

void OLED_Display_Joystick(const Joystick_State *joy)
{
    char x_buf[22];
    char y_buf[22];
    char button_buf[22];

    snprintf(x_buf, sizeof(x_buf), "X%+ld/R%u",
             (long)joy->x, joy->x_raw);
    snprintf(y_buf, sizeof(y_buf), "Y%+ld/R%u",
             (long)joy->y, joy->y_raw);
    snprintf(button_buf, sizeof(button_buf), "SW:%s",
             joy->pressed ? "YES" : "NO");

    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString(x_buf, Font_11x18, White);
    ssd1306_SetCursor(0, 18);
    ssd1306_WriteString(y_buf, Font_11x18, White);
    ssd1306_SetCursor(0, 36);
    ssd1306_WriteString(button_buf, Font_11x18, White);
    ssd1306_UpdateScreen();
}
