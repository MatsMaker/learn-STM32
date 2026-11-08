#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "joystick.h"

void OLED_Display_Init(void);
void OLED_Display_Joystick(const Joystick_State *joy);

#endif
