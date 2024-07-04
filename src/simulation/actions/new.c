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
static bool new_action_shortcut(void) { return IsKeyPressed(KEY_N); }

static void new_action_cancel(Simulation _[static 1],
                              Action new_action[static 1]) {
  new_action->active = false;
}

static void new_action_update(Simulation simulation[static 1],
                              Action new_action[static 1]) {
  if (action_activated(simulation, new_action)) {
    circuit_clear(&simulation->circuit);
    puts("Circuit Cleared");
    action_toggle(simulation, new_action);
  }
}

// Button
static Button NEW_BUTTON = {
    .txt = NEW_BUTTON_TXT,
    BTN_RECT(NEW_BUTTON_X, NEW_BUTTON_Y),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action NEW_ACTION = {
    .data = NULL,
    .active = false,
    .allow_selection = false,
    .prevent_directly_switching_action = false,
    .button = &NEW_BUTTON,
    .shortcut_cond = new_action_shortcut,
    .CANCEL_FN = new_action_cancel,
    .UPDATE_FN = new_action_update,
    .RENDER_FN = NULL,
};

Action *new_action_init(void) {
  // #ifndef NDEBUG
  // action_init(&NEW_ACTION, 0);
  // #endif
  return &NEW_ACTION;
}
