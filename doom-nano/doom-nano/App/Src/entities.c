/* Player and entities (ported from doom-nano for STM32) */
#include "entities.h"
#include "constants.h"

Player create_player(uint8_t x, uint8_t y) {
  Player p = {
    .pos      = create_coords((float) x + 0.5f, (float) y + 0.5f),
    .dir      = create_coords(1.0f, 0.0f),
    .plane    = create_coords(0.0f, -0.66f),
    .velocity = 0.0f,
    .health   = 100,
    .keys     = 0,
  };
  return p;
}

Entity create_entity(uint8_t type, uint8_t x, uint8_t y, uint8_t initialState, uint8_t initialHealth) {
  UID uid = create_uid(type, x, y);
  Coords pos = create_coords((float) x + 0.5f, (float) y + 0.5f);
  Entity new_entity = { uid, pos, initialState, initialHealth, 0, 0 };
  return new_entity;
}

StaticEntity create_static_entity(UID uid, uint8_t x, uint8_t y, bool active) {
  return (StaticEntity){ uid, x, y, active };
}
