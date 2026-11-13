/* Basic game types (ported from doom-nano for STM32) */
#include <math.h>
#include "types.h"
#include "constants.h"

static inline float sq(float value) {
  return value * value;
}

Coords create_coords(float x, float y) {
  return (Coords){ x, y };
}

uint8_t coords_distance(const Coords *a, const Coords *b) {
  float d = sqrtf(sq(a->x - b->x) + sq(a->y - b->y)) * DISTANCE_MULTIPLIER;
  /* AVR silently wrapped this; clamp so far entities stay "far" */
  return d > 255.0f ? 255 : (uint8_t) d;
}

UID create_uid(EType type, uint8_t x, uint8_t y) {
  return (UID) ((((y << LEVEL_WIDTH_BASE) | x) << 4) | type);
}

EType uid_get_type(UID uid) {
  return uid & 0x0F;
}
