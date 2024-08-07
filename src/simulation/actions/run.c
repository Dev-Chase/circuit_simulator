#include "action.h"
#include "button.h"
#include "constants.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h" #include < assert.h>
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

static void run_action_cancel(Simulation _[static 1],
                              Action run_action[static 1]) {
  run_action->active = false;
  puts("Stopped Running");
  run_action->shortcut_cond = run_action_shortcut;
  button_set_state(run_action->button, RUN_TXT, RUN_BG);
}

static void run_action_update(Simulation simulation[static 1],
                              Action run_action[static 1]) {
  if (action_activated(simulation, run_action)) {
    action_toggle(simulation, run_action);

    if (run_action->active) {
      puts("Started Running");
      run_action->shortcut_cond = stop_action_shortcut;
      button_set_state(run_action->button, STOP_TXT, STOP_BG);
    }
  }
  if (IsKeyPressed(KEY_ESCAPE) && run_action->active) {
    run_action_cancel(simulation, run_action);
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
    .active = false,
    .allow_selection = false,
    .prevent_directly_switching_action = true,
    .button = &RUN_BUTTON,
    .shortcut_cond = run_action_shortcut,
    .CANCEL_FN = run_action_cancel,
    .UPDATE_FN = run_action_update,
    .RENDER_FN = NULL,
};

Action *run_action_init(void) {
  // action_init(&RUN_ACTION, sizeof(bool));
  // assert(sizeof(bool) == 1);
  // memset(RUN_ACTION.data, false, sizeof(bool));
  return &RUN_ACTION;
}
