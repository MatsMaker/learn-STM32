/*
 * Game constants (ported from doom-nano for STM32F103 + HAL).
 *
 * Differences from the Arduino version:
 *  - "double" is replaced with "float" (Cortex-M3 has no FPU, and on ARM
 *    double is 64-bit, while on AVR it is 32-bit, so float keeps the
 *    original precision and is much faster).
 *  - Buttons are replaced with the analog joystick (see input.c).
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

/* ---- Input (analog joystick on PB0/PB1, button on PB7) ------------------ */
#define JOY_THRESHOLD        40      /* |axis| (-100..100) to register a direction */
#define JOY_INVERT_X         0       /* set to 1 if left/right are swapped */
#define JOY_INVERT_Y         1      /* set to 1 if up/down are swapped */
#define EXIT_HOLD_MS         1500    /* hold FIRE that long in game -> back to intro */

/* ---- Sound (passive buzzer on PA1 = TIM2_CH2) --------------------------- */
#define SOUND_ENABLED        1

/* ---- GFX settings ------------------------------------------------------- */
#define FRAME_TIME          66.666666f  /* Desired time per frame in ms (~15 fps) */
#define RES_DIVIDER         2           /* Higher -> lower horizontal resolution, faster */
#define Z_RES_DIVIDER       2           /* Z-buffer resolution divider */
#define DISTANCE_MULTIPLIER 20          /* Distances are stored as uint8_t (* this) */
#define MAX_RENDER_DEPTH    12
#define MAX_SPRITE_DEPTH    8

/* ---- Display ------------------------------------------------------------ */
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define HALF_WIDTH          (SCREEN_WIDTH / 2)
#define RENDER_HEIGHT       56          /* raycaster working height (the rest is HUD) */
#define HALF_HEIGHT         (SCREEN_HEIGHT / 2)

#define ZBUFFER_SIZE        (SCREEN_WIDTH / Z_RES_DIVIDER)

/* ---- Level -------------------------------------------------------------- */
#define LEVEL_WIDTH_BASE    6
#define LEVEL_WIDTH         (1 << LEVEL_WIDTH_BASE)
#define LEVEL_HEIGHT        57
#define LEVEL_SIZE          (LEVEL_WIDTH / 2 * LEVEL_HEIGHT)

/* ---- Scenes ------------------------------------------------------------- */
#define INTRO               0
#define GAME_PLAY           1

/* ---- Game --------------------------------------------------------------- */
#define GUN_TARGET_POS        18
#define GUN_SHOT_POS          (GUN_TARGET_POS + 4)

#define ROT_SPEED             0.12f
#define MOV_SPEED             0.2f
#define MOV_SPEED_INV         5.0f        /* 1 / MOV_SPEED */

#define JOGGING_SPEED         0.005f
#define ENEMY_SPEED           0.02f
#define FIREBALL_SPEED        0.2f
#define FIREBALL_ANGLES       45          /* Num of angles per PI */

#define MAX_ENTITIES          10          /* Max num of active entities */
#define MAX_STATIC_ENTITIES   28          /* Max num of entities in sleep mode */

#define MAX_ENTITY_DISTANCE   200         /* * DISTANCE_MULTIPLIER */
#define MAX_ENEMY_VIEW        80          /* * DISTANCE_MULTIPLIER */
#define ITEM_COLLIDER_DIST    6           /* * DISTANCE_MULTIPLIER */
#define ENEMY_COLLIDER_DIST   4           /* * DISTANCE_MULTIPLIER */
#define FIREBALL_COLLIDER_DIST 2          /* * DISTANCE_MULTIPLIER */
#define ENEMY_MELEE_DIST      6           /* * DISTANCE_MULTIPLIER */
#define WALL_COLLIDER_DIST    0.2f

#define ENEMY_MELEE_DAMAGE    8
#define ENEMY_FIREBALL_DAMAGE 20
#define GUN_MAX_DAMAGE        15

/* ---- Helpers ------------------------------------------------------------ */
#ifndef PI
#define PI 3.14159265358979f
#endif

#define MAX(a, b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a, b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b; })

#endif /* CONSTANTS_H */
