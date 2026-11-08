#include <stdio.h>

#include "snake.h"
#include "main.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/* ---- Налаштування ------------------------------------------------------- */
#define CELL            4                       /* розмір клітинки, px      */
#define FIELD_X0        0
#define FIELD_Y0        10                      /* зверху рядок з рахунком  */
#define COLS            (SSD1306_WIDTH / CELL)  /* 32 */
#define ROWS            ((SSD1306_HEIGHT - FIELD_Y0) / CELL) /* 13 */
#define MAX_LEN         (COLS * ROWS)

#define JOY_THRESHOLD   50      /* відхилення стіка, з якого рахуємо напрям  */
#define JOY_INVERT_Y    0      /* 1 — якщо "вгору" дає позитивний Y         */
#define JOY_INVERT_X    0

#define START_PERIOD_MS 250     /* швидкість на старті                       */
#define MIN_PERIOD_MS   80      /* максимальна швидкість                     */
#define SPEEDUP_MS      5       /* прискорення за кожну їжу                  */

typedef enum { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT } Dir;
typedef enum { ST_START, ST_PLAY, ST_OVER } GameState;
typedef struct { uint8_t x, y; } Point;

/* ---- Стан гри ----------------------------------------------------------- */
static Point     body[MAX_LEN];   /* кільцевий буфер, head — голова          */
static uint16_t  head, len;
static Point     food;
static Dir       dir, next_dir;
static GameState game;
static uint16_t  score, best;
static uint32_t  period, last_tick;
static bool      prev_pressed;
static uint32_t  rnd_state = 1;

/* ---- Утиліти ------------------------------------------------------------ */
static uint32_t Rand(void)
{
    rnd_state = rnd_state * 1103515245u + 12345u;
    return rnd_state >> 16;
}

static Point Seg(uint16_t i)     /* i = 0 — голова, len-1 — хвіст */
{
    return body[(head + MAX_LEN - i) % MAX_LEN];
}

static bool OnSnake(Point p, bool skip_tail)
{
    uint16_t n = skip_tail ? len - 1 : len;
    for (uint16_t i = 0; i < n; i++)
    {
        Point s = Seg(i);
        if (s.x == p.x && s.y == p.y)
            return true;
    }
    return false;
}

static void PlaceFood(void)
{
    do {
        food.x = Rand() % COLS;
        food.y = Rand() % ROWS;
    } while (OnSnake(food, false));
}

static void Reset(void)
{
    len  = 3;
    head = 2;
    for (uint16_t i = 0; i < len; i++)
    {
        body[i].x = COLS / 2 - 2 + i;
        body[i].y = ROWS / 2;
    }
    dir = next_dir = DIR_RIGHT;
    score  = 0;
    period = START_PERIOD_MS;
    PlaceFood();
    last_tick = HAL_GetTick();
}

/* ---- Ввід --------------------------------------------------------------- */
static void ReadDirection(const Joystick_State *joy)
{
    int32_t x = JOY_INVERT_X ? -joy->x : joy->x;
    int32_t y = JOY_INVERT_Y ? -joy->y : joy->y;
    int32_t ax = x < 0 ? -x : x;
    int32_t ay = y < 0 ? -y : y;
    Dir d;

    if (ax < JOY_THRESHOLD && ay < JOY_THRESHOLD)
        return;

    if (ax > ay) d = (x > 0) ? DIR_RIGHT : DIR_LEFT;
    else         d = (y > 0) ? DIR_DOWN  : DIR_UP;

    /* Не дозволяємо розвернутись у себе */
    if ((d + 2) % 4 != dir)
        next_dir = d;
}

/* ---- Логіка ------------------------------------------------------------- */
static void Step(void)
{
    Point h = Seg(0);
    dir = next_dir;

    switch (dir)
    {
    case DIR_UP:    if (h.y == 0)        goto dead; h.y--; break;
    case DIR_DOWN:  if (h.y == ROWS - 1) goto dead; h.y++; break;
    case DIR_LEFT:  if (h.x == 0)        goto dead; h.x--; break;
    case DIR_RIGHT: if (h.x == COLS - 1) goto dead; h.x++; break;
    }

    bool eat = (h.x == food.x && h.y == food.y);

    /* Хвіст звільнить клітинку, якщо не їмо — тому в неї можна заходити */
    if (OnSnake(h, !eat))
        goto dead;

    head = (head + 1) % MAX_LEN;
    body[head] = h;

    if (eat)
    {
        len++;
        score++;
        if (period > MIN_PERIOD_MS + SPEEDUP_MS) period -= SPEEDUP_MS;
        else                                     period = MIN_PERIOD_MS;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        if (len < MAX_LEN) PlaceFood();
        else               goto dead;   /* поле заповнене — перемога :) */
    }
    return;

dead:
    if (score > best) best = score;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    game = ST_OVER;
}

/* ---- Малювання ---------------------------------------------------------- */
static void DrawCell(Point p, bool filled)
{
    uint8_t x = FIELD_X0 + p.x * CELL;
    uint8_t y = FIELD_Y0 + p.y * CELL;
    if (filled)
        ssd1306_FillRectangle(x, y, x + CELL - 2, y + CELL - 2, White);
    else
        ssd1306_DrawRectangle(x, y, x + CELL - 2, y + CELL - 2, White);
}

static void DrawGame(void)
{
    char buf[32];

    ssd1306_Fill(Black);
    snprintf(buf, sizeof(buf), "Score:%u  Best:%u", score, best);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString(buf, Font_6x8, White);
    ssd1306_Line(0, FIELD_Y0 - 2, SSD1306_WIDTH - 1, FIELD_Y0 - 2, White);

    for (uint16_t i = 0; i < len; i++)
        DrawCell(Seg(i), true);
    DrawCell(food, false);

    ssd1306_UpdateScreen();
}

static void DrawText(const char *title, const char *line2)
{
    char buf[22];

    ssd1306_Fill(Black);
    ssd1306_SetCursor((SSD1306_WIDTH - 11 * 5) / 2, 6);
    ssd1306_WriteString((char *)title, Font_11x18, White);
    ssd1306_SetCursor(0, 32);
    ssd1306_WriteString((char *)line2, Font_6x8, White);
    snprintf(buf, sizeof(buf), "Best: %u", best);
    ssd1306_SetCursor(0, 44);
    ssd1306_WriteString(buf, Font_6x8, White);
    ssd1306_SetCursor(0, 56);
    ssd1306_WriteString("Press stick to play", Font_6x8, White);
    ssd1306_UpdateScreen();
}

/* ---- Публічне API ------------------------------------------------------- */
void Snake_Init(void)
{
    game = ST_START;
    DrawText("SNAKE", "Move with joystick");
}

void Snake_Process(const Joystick_State *joy)
{
    /* Фронт натискання кнопки */
    bool click = joy->pressed && !prev_pressed;
    prev_pressed = joy->pressed;

    /* Шум АЦП + час — непоганий сід для ГВЧ */
    rnd_state += joy->x_raw ^ (joy->y_raw << 8) ^ HAL_GetTick();

    switch (game)
    {
    case ST_START:
    case ST_OVER:
        if (click)
        {
            Reset();
            game = ST_PLAY;
            DrawGame();
        }
        break;

    case ST_PLAY:
        ReadDirection(joy);
        if (HAL_GetTick() - last_tick >= period)
        {
            last_tick += period;
            Step();
            if (game == ST_OVER)
            {
                char buf[22];
                snprintf(buf, sizeof(buf), "Score: %u", score);
                DrawText("OOPS!", buf);
            }
            else
            {
                DrawGame();
            }
        }
        break;
    }
}
