#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t x_raw;
    uint16_t y_raw;
    int32_t  x;        /* -100..100 */
    int32_t  y;        /* -100..100 */
    bool     pressed;
} Joystick_State;

void Joystick_Init(void);
void Joystick_Update(void);
const Joystick_State *Joystick_Get(void);

#endif
