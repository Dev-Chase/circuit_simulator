#include "action.h"
#include "button.h"
#include "circuit.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Default Action Data
#define NEW_BUTTON_X GRID_VAL_TO_COORD(1)
#define NEW_BUTTON_Y GRID_VAL_TO_COORD(1)
#define NEW_BUTTON_TXT "New"

// Functions
static bool new_action_shortcut(void) {
  return IsKeyPressed(KEY_N) &&
         (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
}

static void new_action_update(Simulation simulation[static 1],
                              Action new_action[static 1]) {
  if (action_activated(new_action)) {
    circuit_clear(&simulation->circuit);
    puts("Circuit Cleared");
  }
}

// Button
static Button NEW_BUTTON = {
    .txt = NEW_BUTTON_TXT,
    RECT(NEW_BUTTON_X, NEW_BUTTON_Y, BTN_WIDTH, BTN_HEIGHT),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action NEW_ACTION = {
    .data = NULL,
    .button = &NEW_BUTTON,
    .shortcut_cond = new_action_shortcut,
    .UPDATE_FN = new_action_update,
    .RENDER_FN = NULL,
};

Action *new_action_init(void) {
  // #ifndef NDEBUG
  action_init(&NEW_ACTION, 0);
  // #endif
  return &NEW_ACTION;
}
