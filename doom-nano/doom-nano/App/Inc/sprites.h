/* Sprites, font and gradients (ported from doom-nano for STM32) */
#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>



#define bmp_font_width   24  // in bytes
#define bmp_font_height  6
#define CHAR_MAP         " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.,-_(){}[]#"
#define CHAR_WIDTH       4
#define CHAR_HEIGHT      6
extern const uint8_t bmp_font[];

#define BMP_LOGO_WIDTH  72
#define BMP_LOGO_HEIGHT 47
extern const uint8_t bmp_logo_bits[];

#define BMP_GUN_WIDTH   32
#define BMP_GUN_HEIGHT  32
extern const uint8_t bmp_gun_bits[];
extern const uint8_t bmp_gun_mask[];

#define BMP_FIRE_WIDTH  24
#define BMP_FIRE_HEIGHT 20
extern const uint8_t bmp_fire_bits[];

#define BMP_IMP_WIDTH   32
#define BMP_IMP_HEIGHT  32
#define BMP_IMP_COUNT   5
extern const uint8_t bmp_imp_bits[];
extern const uint8_t bmp_imp_mask[];

#define BMP_FIREBALL_WIDTH 16
#define BMP_FIREBALL_HEIGHT 16
extern const uint8_t bmp_fireball_bits[];
extern const uint8_t bmp_fireball_mask[];

#define BMP_DOOR_WIDTH    32
#define BMP_DOOR_HEIGHT   32
extern const uint8_t bmp_door_bits[];

#define BMP_ITEMS_WIDTH   16
#define BMP_ITEMS_HEIGHT  16
#define BMP_ITEMS_COUNT   2
extern const uint8_t bmp_items_bits[];
extern const uint8_t bmp_items_mask[];

// Gradient for lighting.
// Note: Width and height are in bytes
#define GRADIENT_WIDTH  2
#define GRADIENT_HEIGHT 8
#define GRADIENT_COUNT  8
#define GRADIENT_WHITE  7
#define GRADIENT_BLACK  0
extern const uint8_t gradient[];



#endif /* SPRITES_H */
