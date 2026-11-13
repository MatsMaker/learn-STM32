/*
 * Sound (ported from doom-nano).
 *
 * Arduino: Timer1 toggled pin 9 at the note frequency, Timer2 ISR at ~140 Hz
 * stepped through the sound data.
 * STM32:   TIM2_CH2 (PA1) toggles at the note frequency, TIM3 ISR at ~139 Hz
 *          steps through the sound data. Both timers are configured here
 *          directly (they are not part of the CubeMX configuration), PA1 is
 *          switched from GPIO output to alternate function.
 */
#include "main.h"
#include "sound.h"
#include "constants.h"

const uint8_t shoot_snd[] = { 0x10, 0x10 , 0x10 , 0x6e , 0x2a , 0x20 , 0x28 , 0x28 , 0x9b , 0x28 , 0x20 , 0x20 , 0x21 , 0x57 , 0x20 , 0x20 , 0x20 , 0x67 , 0x20 , 0x20 , 0x29 , 0x20 , 0x73 , 0x20 , 0x20 , 0x20 , 0x89};
const uint8_t get_key_snd[] = {0x24, 0x24, 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 };
const uint8_t hit_wall_snd[] = { 0x83 , 0x83 , 0x82 , 0x8e , 0x8a , 0x89 , 0x86 , 0x84};
const uint8_t walk1_snd[] = { 0x8f, 0x8e, 0x8e};
const uint8_t walk2_snd[] = { 0x84, 0x87, 0x84};
const uint8_t medkit_snd[] = {0x55 , 0x20 , 0x3a , 0x3a , 0x3a , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x33 , 0x33 , 0x33 , 0x33 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x26 , 0x26 , 0x26 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x15 , 0x15 , 0x15 , 0x15 , 0x15 , 0x15, 0x15};

#if SOUND_ENABLED

static volatile const uint8_t *snd_ptr = 0;
static volatile uint8_t snd_len = 0;
static volatile uint8_t idx = 0;
static volatile bool sound = false;

#define TONE_TIMER_CLK   1000000UL   /* TIM2 counter clock after prescaler */

void sound_init(void) {
  GPIO_InitTypeDef gpio = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_TIM3_CLK_ENABLE();

  /* PA1 -> TIM2_CH2 */
  gpio.Pin = GPIO_PIN_1;
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &gpio);

  /* TIM2: tone generator, CH2 toggles on every update -> f = clk / (2 * (ARR + 1)) */
  TIM2->CR1 = 0;
  TIM2->PSC = (SystemCoreClock / TONE_TIMER_CLK) - 1;   /* APB1 timer clock = 72 MHz */
  TIM2->ARR = 1000;
  TIM2->CCR2 = 0;
  TIM2->CCMR1 = (TIM2->CCMR1 & ~(TIM_CCMR1_OC2M | TIM_CCMR1_CC2S)) | (TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1); /* toggle */
  TIM2->CCER &= ~TIM_CCER_CC2E;                         /* output off until a sound plays */
  TIM2->EGR = TIM_EGR_UG;

  /* TIM3: sequencer tick 72 MHz / 7200 / 72 = 138.9 Hz (original: 139.5 Hz) */
  TIM3->CR1 = 0;
  TIM3->PSC = 7200 - 1;
  TIM3->ARR = 72 - 1;
  TIM3->EGR = TIM_EGR_UG;
  TIM3->SR = 0;
  TIM3->DIER = TIM_DIER_UIE;
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  TIM3->CR1 = TIM_CR1_CEN;
}

void playSound(const uint8_t *snd, uint8_t len) {
  __disable_irq();
  snd_ptr = snd;
  snd_len = len;
  idx = 0;
  sound = true;
  __enable_irq();
}

bool sound_is_playing(void) {
  return sound;
}

static void setFrequency(uint16_t freq) {
  uint32_t arr = TONE_TIMER_CLK / 2 / freq;
  if (arr < 2) arr = 2;
  if (arr > 0xFFFF) arr = 0xFFFF;

  TIM2->ARR = arr - 1;
  if (TIM2->CNT >= arr) TIM2->CNT = 0;
  TIM2->CCER |= TIM_CCER_CC2E;
  TIM2->CR1 |= TIM_CR1_CEN;
}

static void off(void) {
  TIM2->CR1 &= ~TIM_CR1_CEN;
  TIM2->CCER &= ~TIM_CCER_CC2E;
}

void TIM3_IRQHandler(void) {
  if (TIM3->SR & TIM_SR_UIF) {
    TIM3->SR = ~TIM_SR_UIF;

    if (sound) {
      if (idx < snd_len && snd_ptr[idx] != 0) {
        uint16_t freq = 1192030UL / (60UL * snd_ptr[idx]); /* 1193181 */
        setFrequency(freq);
        idx++;
      } else {
        idx = 0;
        off();
        sound = false;
      }
    }
  }
}

#else /* !SOUND_ENABLED */

void sound_init(void) {}
void playSound(const uint8_t *snd, uint8_t len) { (void) snd; (void) len; }
bool sound_is_playing(void) { return false; }

#endif /* SOUND_ENABLED */
