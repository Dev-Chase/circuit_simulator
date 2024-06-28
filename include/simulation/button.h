#ifndef BUTTON_H
#define BUTTON_H

#include "constants.h"
#include "raylib.h"
#include "utils.h"
#ifdef _cplusplus
extern "C" {
#endif

#define BTN_GRID_WIDTH 3
#define BTN_BORDER_THICKNESS 3
#define BTN_WIDTH GRID_GAP *BTN_GRID_WIDTH
#define BTN_HEIGHT GRID_GAP
#define BTN_GRID_GAP 0.5
#define BTN_FONT_SIZE 17

// Corner Drawing
#define BTN_CORNER_ROUNDNESS 0.2F
#define BTN_CORNER_SEGMENTS 25

// Defaults
#define TRANS_GREEN                                                            \
  (Color) { 0, 228, 48, 180 }
#define TRANS_RED                                                              \
  (Color) { 230, 41, 55, 190 }

// Utility Macros
#define BTN_RECT(x, y) RECT(x, y, BTN_WIDTH, BTN_HEIGHT)

typedef struct Button {
  const char *txt;
  const Rectangle RECT;
  Color fg;
  Color bg;
} Button;

bool button_is_clicked(Button button[static 1]);
void button_render(const Button button[static 1]);
void button_set_state(Button button[static 1], const char *txt, Color bg);

#ifdef _cplusplus
}
#endif
#endif
