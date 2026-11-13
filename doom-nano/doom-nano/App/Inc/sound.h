/*
 * Sound (ported from doom-nano).
 *
 * For infos how this works see
 * http://fabiensanglard.net/gebbwolf3d_v2.1.pdf chapter 4.9.5
 * http://www.shikadi.net/moddingwiki/Inverse_Frequency_Sound_format
 * http://www.shikadi.net/moddingwiki/AudioT_Format
 *
 * STM32 version: passive buzzer on PA1 driven by TIM2_CH2 (toggle mode),
 * the sound "sequencer" runs from the TIM3 interrupt at ~140 Hz.
 */
#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include <stdbool.h>

#define GET_KEY_SND_LEN   90
#define SHOOT_SND_LEN     27
#define HIT_WALL_SND_LEN  8
#define WALK1_SND_LEN     3
#define WALK2_SND_LEN     3
#define MEDKIT_SND_LEN    69

extern const uint8_t shoot_snd[];
extern const uint8_t get_key_snd[];
extern const uint8_t hit_wall_snd[];
extern const uint8_t walk1_snd[];
extern const uint8_t walk2_snd[];
extern const uint8_t medkit_snd[];

void sound_init(void);
void playSound(const uint8_t *snd, uint8_t len);
bool sound_is_playing(void);

#endif /* SOUND_H */
