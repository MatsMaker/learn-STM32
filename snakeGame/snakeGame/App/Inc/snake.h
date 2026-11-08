#ifndef SNAKE_H
#define SNAKE_H

#include "joystick.h"

void Snake_Init(void);
/* Викликати якомога частіше з головного циклу */
void Snake_Process(const Joystick_State *joy);

#endif
