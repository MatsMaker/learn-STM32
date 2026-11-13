/*
 * Input (ported from doom-nano).
 *
 * The Arduino version used 5 buttons. Here the analog joystick from the
 * flightStick project is used instead:
 *   Y axis (PB1 / ADC1_IN9)  -> up / down    (move)
 *   X axis (PB0 / ADC1_IN8)  -> left / right (turn)
 *   button (PB7 / JOY_SW)    -> fire
 */
#include "input.h"
#include "joystick.h"
#include "constants.h"

static const Joystick_State *joy;

void input_setup(void) {
  Joystick_Init();
  joy = Joystick_Get();
}

void input_update(void) {
  Joystick_Update();
}

static inline int32_t axis_x(void) {
  return JOY_INVERT_X ? -joy->x : joy->x;
}

static inline int32_t axis_y(void) {
  return JOY_INVERT_Y ? -joy->y : joy->y;
}

bool input_left(void) {
  return axis_x() <= -JOY_THRESHOLD;
}

bool input_right(void) {
  return axis_x() >= JOY_THRESHOLD;
}

bool input_up(void) {
  return axis_y() >= JOY_THRESHOLD;
}

bool input_down(void) {
  return axis_y() <= -JOY_THRESHOLD;
}

bool input_fire(void) {
  return joy->pressed;
}
