/*
 * Rendering helpers (ported from doom-nano for STM32).
 *
 * The original used a slim Adafruit SSD1306 driver over AVR TWI. Here the
 * game draws into its own frame buffer (display_buf) and pushes it with the
 * stm32-ssd1306 library (I2C1, configured by CubeMX) once per frame.
 */
#include <string.h>
#include "main.h"
#include "display.h"
#include "sprites.h"
#include "ssd1306.h"

/* Bits are read from left to right */
static const uint8_t bit_mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
#define read_bit(b, n)      (((b) & bit_mask[(n)]) ? 1 : 0)

uint8_t display_buf[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

/* We don't handle more than MAX_RENDER_DEPTH depth, so we can safety store
   z values in a byte with 1 decimal and save some memory */
uint8_t zbuffer[ZBUFFER_SIZE];

/* FPS control */
float delta = 1;
static uint32_t lastFrameTime = 0;
static bool inverted = false;

void setupDisplay(void) {
  ssd1306_Init();
  memset(display_buf, 0, sizeof(display_buf));
  display_update();

  /* initialize z buffer */
  memset(zbuffer, 0xFF, ZBUFFER_SIZE);
}

void display_update(void) {
  ssd1306_FillBuffer(display_buf, sizeof(display_buf));
  ssd1306_UpdateScreen();
}

void display_invert(bool invert) {
  /* Only talk to the display when the state actually changes */
  if (invert != inverted) {
    ssd1306_WriteCommand(invert ? 0xA7 : 0xA6);
    inverted = invert;
  }
}

void display_clearRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  for (uint16_t i = x; i < x + w; i++) {
    for (uint16_t j = y; j < y + h; j++) {
      drawPixel(i, j, 0, false);
    }
  }
}

/* Adafruit-like drawBitmap: rows of bytes, MSB first. Only set bits are drawn */
void display_drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, bool color) {
  int16_t byteWidth = (w + 7) / 8;
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7) byte <<= 1;
      else       byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80) drawPixel(x + i, y, color, false);
    }
  }
}

/* Adds a delay to limit play to specified fps.
   Calculates also delta to keep movement consistent in lower framerates */
void fps(void) {
  while ((float) (HAL_GetTick() - lastFrameTime) < FRAME_TIME);
  delta = (float) (HAL_GetTick() - lastFrameTime) / FRAME_TIME;
  lastFrameTime = HAL_GetTick();
}

float getActualFps(void) {
  return 1000.0f / (FRAME_TIME * delta);
}

/* Faster way to render vertical bits */
void drawByte(uint8_t x, uint8_t y, uint8_t b) {
  display_buf[(y / 8) * SCREEN_WIDTH + x] = b;
}

bool getGradientPixel(uint8_t x, uint8_t y, uint8_t i) {
  if (i == 0) return 0;
  if (i >= GRADIENT_COUNT - 1) return 1;

  uint8_t index = i * GRADIENT_WIDTH * GRADIENT_HEIGHT              /* gradient index */
                  + y * GRADIENT_WIDTH % (GRADIENT_WIDTH * GRADIENT_HEIGHT) /* y byte offset */
                  + x / GRADIENT_HEIGHT % GRADIENT_WIDTH;           /* x byte offset */

  /* return the bit based on x */
  return read_bit(gradient[index], x % 8);
}

void fadeScreen(uint8_t intensity, bool color) {
  for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
      if (getGradientPixel(x, y, intensity))
        drawPixel(x, y, color, false);
    }
  }
}

/* Faster drawPixel. Avoids some checks to make it faster. */
void drawPixel(int16_t x, int16_t y, bool color, bool raycasterViewport) {
  /* prevent write out of screen buffer */
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= (raycasterViewport ? RENDER_HEIGHT : SCREEN_HEIGHT)) {
    return;
  }

  if (color) {
    display_buf[x + (y / 8) * SCREEN_WIDTH] |= (1 << (y & 7));
  } else {
    display_buf[x + (y / 8) * SCREEN_WIDTH] &= ~(1 << (y & 7));
  }
}

/* For raycaster only.
   Custom draw Vertical lines that fills with a pattern to simulate
   different brightness. Affected by RES_DIVIDER */
void drawVLine(uint8_t x, int8_t start_y, int8_t end_y, uint8_t intensity) {
  int8_t y;
  int8_t lower_y = MAX(MIN(start_y, end_y), 0);
  int8_t higher_y = MIN(MAX(start_y, end_y), RENDER_HEIGHT - 1);
  uint8_t c;
  uint8_t bp;
  uint8_t b;

  if (lower_y > higher_y) return;

  for (c = 0; c < RES_DIVIDER; c++) {
    y = lower_y;
    b = 0;
    bp = 0;
    while (y <= higher_y) {
      bp = y % 8;
      b = b | (getGradientPixel(x + c, y, intensity) << bp);

      if (bp == 7) {
        /* write the whole byte */
        drawByte(x + c, y, b);
        b = 0;
      }

      y++;
    }

    /* draw last byte */
    if (bp != 7) {
      drawByte(x + c, y - 1, b);
    }
  }
}

/* Custom drawBitmap method with scale support, mask, zindex and pattern filling */
void drawSprite(
  int16_t x, int16_t y,
  const uint8_t bitmap[], const uint8_t mask[],
  int16_t w, int16_t h,
  uint8_t sprite,
  float distance
) {
  uint8_t tw = (float) w / distance;
  uint8_t th = (float) h / distance;
  uint8_t byte_width = w / 8;
  uint8_t pixel_size = MAX(1, (int) (1.0f / distance));
  uint16_t sprite_offset = byte_width * h * sprite;

  bool pixel;
  bool maskPixel;

  /* Don't draw the whole sprite if its center is hidden by z buffer.
     Not checked per pixel for performance reasons */
  int16_t zx = (x + tw / 2) / Z_RES_DIVIDER;
  zx = MIN(MAX(zx, 0), ZBUFFER_SIZE - 1);
  if (zbuffer[zx] < distance * DISTANCE_MULTIPLIER) {
    return;
  }

  for (uint8_t ty = 0; ty < th; ty += pixel_size) {
    /* Don't draw out of screen */
    if (y + ty < 0 || y + ty >= RENDER_HEIGHT) {
      continue;
    }

    uint8_t sy = ty * distance; /* The y from the sprite */

    for (uint8_t tx = 0; tx < tw; tx += pixel_size) {
      uint8_t sx = tx * distance; /* The x from the sprite */
      uint16_t byte_offset = sprite_offset + sy * byte_width + sx / 8;

      /* Don't draw out of screen */
      if (x + tx < 0 || x + tx >= SCREEN_WIDTH) {
        continue;
      }

      maskPixel = read_bit(mask[byte_offset], sx % 8);

      if (maskPixel) {
        pixel = read_bit(bitmap[byte_offset], sx % 8);
        for (uint8_t ox = 0; ox < pixel_size; ox++) {
          for (uint8_t oy = 0; oy < pixel_size; oy++) {
            drawPixel(x + tx + ox, y + ty + oy, pixel, true);
          }
        }
      }
    }
  }
}

/* Draw a single character.
   Made for a custom font with some useful sprites. Char size 4 x 6 */
void drawChar(int8_t x, int8_t y, char ch) {
  static const char char_map[] = CHAR_MAP;
  uint8_t c = 0;
  uint8_t n;
  uint8_t bOffset;
  uint8_t b;
  uint8_t line = 0;

  /* Find the character */
  while (char_map[c] != ch && char_map[c] != '\0') c++;

  bOffset = c / 2;
  for (; line < CHAR_HEIGHT; line++) {
    b = bmp_font[line * bmp_font_width + bOffset];
    for (n = 0; n < CHAR_WIDTH; n++)
      if (read_bit(b, (c % 2 == 0 ? 0 : 4) + n))
        drawPixel(x + n, y + line, 1, false);
  }
}

/* Draw a string */
void drawText(int8_t x, int8_t y, const char *txt, uint8_t space) {
  uint8_t pos = x;
  uint8_t i = 0;
  char ch;
  while (pos < SCREEN_WIDTH && (ch = txt[i]) != '\0') {
    drawChar(pos, y, ch);
    i++;
    pos += CHAR_WIDTH + space;
  }
}

/* Draw an integer (3 digit max!) */
void drawNumber(uint8_t x, uint8_t y, uint8_t num) {
  char buf[4]; /* 3 char + \0 */
  char *p = buf + sizeof(buf) - 1;
  *p = '\0';
  do {
    *--p = '0' + (num % 10);
    num /= 10;
  } while (num > 0);
  drawText(x, y, p, 1);
}
