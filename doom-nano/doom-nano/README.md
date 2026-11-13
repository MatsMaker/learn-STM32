# doom-nano для STM32F103C8 (Blue Pill)

**Doom Nano** — це маленький 3D-рушій з raycasting для мікроконтролерів, схожий на Wolfenstein 3D, зі спрайтами в стилі Doom.
Гравець ходить лабіринтом, стріляє в імпів, які кидаються вогняними кулями, і збирає аптечки та ключі.
Картинка виводиться на монохромний OLED 128x64. Глибина передається дизерингом, є z-буфер для спрайтів, HUD з
здоров'ям і ключами, ефекти розгойдування зброї, спалаху екрана та плавного затухання.

Цей проект — порт оригінальної гри з Arduino Nano (ATmega328P, 16 МГц, 2 КБ RAM) на STM32F103C8
(Cortex-M3, 72 МГц, 20 КБ RAM) у STM32CubeIDE, з керуванням аналоговим джойстиком.

![doom-nano на STM32](../../doom-pr.gif)

**Джерело гри:** [github.com/daveruiz/doom-nano](https://github.com/daveruiz/doom-nano), автор Dave Ruiz.
Оригінальні файли лежать поруч, у [../doom-nano-master](../doom-nano-master).

CubeMX-конфігурація (`doom-nano.ioc`) взята з проекту `flightStick_bluetooth`, периферія не змінювалась.

## Підключення

| Що                    | Пін        | Примітка                                  |
|-----------------------|------------|-------------------------------------------|
| OLED SSD1306 128x64   | PB8 / PB9  | I2C1 SCL / SDA, 400 кГц, адреса 0x3C      |
| Джойстик X (VRx)      | PB0        | ADC1_IN8 → поворот вліво/вправо           |
| Джойстик Y (VRy)      | PB1        | ADC1_IN9 → рух вперед/назад               |
| Кнопка джойстика (SW) | PB7        | JOY_SW, pull-up → постріл                 |
| Пасивний бузер        | PA1        | TIM2_CH2 (замість світлодіода)            |
| UART1 (HC-05/06)      | PA9 / PA10 | налаштований, гра його не використовує    |

## Керування

- джойстик вперед/назад — рух, вліво/вправо — поворот;
- кнопка — постріл / старт;
- кнопку затиснути на 1.5 с — вихід у заставку (в оригіналі для цього було LEFT+RIGHT).

Напрямки осей інвертуються через `JOY_INVERT_X` / `JOY_INVERT_Y` у `App/Inc/constants.h`,
звук вимикається через `SOUND_ENABLED 0`. Калібрування осей задається через `X_MIN/X_MAX/Y_MIN/Y_MAX` у `App/Src/joystick.c`.

## Файли оригіналу → файли порту

| Оригінал (`doom-nano-master`)          | Порт (`App/`)                         | Що змінилось |
|----------------------------------------|---------------------------------------|--------------|
| `doom-nano.ino`                        | `Src/doom.c`, `Inc/doom.h`            | `setup()`/`loop()` → `doom_setup()`/`doom_loop()`, C замість C++ |
| `constants.h`                          | `Inc/constants.h`                     | `constexpr` → `#define`, налаштування джойстика і звуку |
| `display.h`                            | `Src/display.c`, `Inc/display.h`      | власний буфер кадру, вивід через `stm32-ssd1306` |
| `SSD1306.cpp/.h`, `TWI_Master.cpp/.h`  | `Lib/stm32-ssd1306-master` (HAL I2C)  | замінено бібліотекою з flightStick |
| `input.cpp/.h`                         | `Src/input.c`, `Src/joystick.c`       | 5 кнопок / SNES → аналоговий джойстик |
| `sound.h`                              | `Src/sound.c`, `Inc/sound.h`          | AVR Timer1/Timer2 → TIM2 (тон) + TIM3 (секвенсор) |
| `entities.cpp/.h`                      | `Src/entities.c`, `Inc/entities.h`    | `create_player` — функція замість макросу |
| `types.cpp/.h`                         | `Src/types.c`, `Inc/types.h`          | `double` → `float` |
| `sprites.h`                            | `Src/sprites.c`, `Inc/sprites.h`      | `PROGMEM`-масиви → `const`, дані без змін |
| `level.h`                              | `Src/level.c`, `Inc/level.h`          | `PROGMEM`-масив → `const`, карта без змін |

## Основні зміни при портуванні

1. **Проект CubeIDE.** Налаштування скопійовано з `flightStick_bluetooth`. Debug-збірка з `-O2`, стек 0x800.
   Гра викликається з `main.c` тільки в секціях `USER CODE`, тож перегенерація CubeMX її не стирає.
2. **C++/Arduino → C/HAL.**
   - `PROGMEM`/`pgm_read_byte` → `const`;
   - `millis()`/`delay()` → `HAL_GetTick()`/`HAL_Delay()`;
   - `F("...")` → звичайні рядки;
   - перевантаження функцій → окремі функції (`drawText`, `drawNumber`).
3. **`double` → `float`.** На AVR `double` і так 32-бітний, а на Cortex-M3 без FPU 64-бітний `double` дуже повільний.
4. **Дисплей.** Гра малює у свій буфер 128x64, який раз на кадр передається в SSD1306 через I2C1. Інверсія екрана йде командою `0xA6/0xA7`.
5. **Керування.** Модуль джойстика з flightStick: фільтр зменшено, щоб не було затримки, а переривання DMA АЦП вимкнено, щоб не гальмували рендер.
6. **Звук.** TIM2_CH2 у режимі toggle генерує тон на PA1, TIM3 приблизно 139 разів на секунду крокує по нотах. Таймери налаштовуються в коді, без CubeMX.
7. **Виправлені баги оригіналу:**
   - вихід за межі масиву в `removeEntity`;
   - неправильний індекс z-буфера для спрайтів;
   - змінні без початкового значення;
   - `sortEntities` без `return`;
   - при новій грі не скидалися сутності;
   - автостарт гри при затиснутій кнопці;
   - переповнення яскравості далеких стін.
8. **АЦП.** У `.ioc` другий канал виправлено на `ADC_CHANNEL_9` (вісь Y).

## Структура

- `App/Src/doom.c` — ігрова логіка, рейкастер, HUD
- `App/Src/display.c` — малювання і вивід на OLED
- `App/Src/input.c` + `joystick.c` — джойстик
- `App/Src/sound.c` — звук на PA1
- `App/Src/sprites.c`, `level.c` — спрайти й карта

Якщо колись увімкнеш TIM2/TIM3 у `.ioc`, прибери `TIM3_IRQHandler` із `sound.c`.

## Ліцензія та автори

Гра, рушій, спрайти й карта взяті з [daveruiz/doom-nano](https://github.com/daveruiz/doom-nano).
Спрайти — зі [spriters-resource.com](https://www.spriters-resource.com), алгоритм raycasting — з [lodev.org/cgtutor](https://lodev.org/cgtutor).
Бібліотека дисплея — [stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) (MIT).
