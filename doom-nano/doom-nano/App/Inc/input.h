/* Input (ported from doom-nano: buttons -> analog joystick) */
#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

void input_setup(void);
void input_update(void);   /* sample the joystick; call once per frame */
bool input_up(void);
bool input_down(void);
bool input_left(void);
bool input_right(void);
bool input_fire(void);

#endif /* INPUT_H */
