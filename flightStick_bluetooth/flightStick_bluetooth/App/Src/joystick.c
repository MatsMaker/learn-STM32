#include "joystick.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_CHANNELS    2
#define FILTER_LEN      8
#define DEADZONE        5

/* Межі калібрування для кожної осі (заміряйте для свого модуля) */
#define X_MIN           970
#define X_MAX           4023
#define Y_MIN           50
#define Y_MAX           4000

static volatile uint16_t dma_buf[ADC_CHANNELS];  /* [0]=X (PB0/ADC_IN8), [1]=Y (PB1/ADC_IN9) */
static uint16_t x_hist[FILTER_LEN];
static uint16_t y_hist[FILTER_LEN];
static uint8_t  hist_idx;
static uint16_t x_center;
static uint16_t y_center;
static Joystick_State state;
static uint8_t pressed_stable_count;

static uint16_t Average(const uint16_t *hist)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < FILTER_LEN; i++)
    {
        sum += hist[i];
    }
    return (uint16_t)(sum / FILTER_LEN);
}

static int16_t Map(uint16_t raw, uint16_t min, uint16_t center, uint16_t max)
{
    int32_t v;

    if (raw >= center)
        v = (int32_t)(raw - center) * 100 / (int32_t)(max - center);
    else
        v = -(int32_t)(center - raw) * 100 / (int32_t)(center - min);

    if (v > 100)  v = 100;
    if (v < -100) v = -100;
    if (v > -DEADZONE && v < DEADZONE) v = 0;

    return (int16_t)v;
}

void Joystick_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)dma_buf, ADC_CHANNELS);
    HAL_Delay(50);

    /* Заповнити історію поточними значеннями, щоб фільтр не починав з нуля */
    for (uint8_t i = 0; i < FILTER_LEN; i++)
    {
        x_hist[i] = dma_buf[0];
        y_hist[i] = dma_buf[1];
    }

    /* Центр — це положення спокою при старті */
    x_center = Average(x_hist);
    y_center = Average(y_hist);
}

void Joystick_Update(void)
{
    x_hist[hist_idx] = dma_buf[0];
    y_hist[hist_idx] = dma_buf[1];
    hist_idx = (hist_idx + 1) % FILTER_LEN;

    state.x_raw = Average(x_hist);
    state.y_raw = Average(y_hist);
    state.x = Map(state.x_raw, X_MIN, x_center, X_MAX);
    state.y = Map(state.y_raw, Y_MIN, y_center, Y_MAX);
    if (HAL_GPIO_ReadPin(JOY_SW_GPIO_Port, JOY_SW_Pin) == GPIO_PIN_RESET)
    {
        if (pressed_stable_count < 5)
            pressed_stable_count++;
    }
    else
    {
        pressed_stable_count = 0;
    }

    state.pressed = (pressed_stable_count >= 5);
}

const Joystick_State *Joystick_Get(void)
{
    return &state;
}
