#include "action.h"
#include "button.h"
#include "constants.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default Action Data
#define RUN_X GRID_VAL_TO_COORD(5)
#define RUN_Y GRID_VAL_TO_COORD(1)
#define RUN_BG TRANS_GREEN
#define RUN_TXT "Run"
#define STOP_BG TRANS_RED
#define STOP_TXT "Stop"

// Functions
static bool run_action_shortcut(void) {
  return (IsKeyPressed(KEY_R) &&
          (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) ||
         IsKeyPressed(KEY_SPACE);
}

static bool stop_action_shortcut(void) {
  return (IsKeyPressed(KEY_S) &&
          (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) ||
         IsKeyPressed(KEY_SPACE);
}

static void run_action_update(Simulation _[static 1],
                              Action run_action[static 1]) {
  if (action_activated(run_action)) {
    bool *btn_state = (bool *)run_action->data;
    *btn_state = !*btn_state;

    if (*btn_state) {
      run_action->shortcut_cond = stop_action_shortcut;
      button_set_state(run_action->button, STOP_TXT, STOP_BG);
    } else {
      run_action->shortcut_cond = run_action_shortcut;
      button_set_state(run_action->button, RUN_TXT, RUN_BG);
    }
  }
}

// Button
static Button RUN_BUTTON = {
    .txt = RUN_TXT,
    BTN_RECT(RUN_X, RUN_Y),
    .fg = FG_COLOUR,
    .bg = RUN_BG,
};

static Action RUN_ACTION = {
    .data = NULL,
    .button = &RUN_BUTTON,
    .shortcut_cond = run_action_shortcut,
    .UPDATE_FN = run_action_update,
    .RENDER_FN = NULL,
};

Action *run_action_init(void) {
  action_init(&RUN_ACTION, sizeof(bool));
  assert(sizeof(bool) == 1);
  memset(RUN_ACTION.data, false, sizeof(bool));
  return &RUN_ACTION;
}
