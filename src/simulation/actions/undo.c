#include "action.h"
#include "button.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// Default Action Data
#define UNDO_BUTTON_X GRID_VAL_TO_COORD(17)
#define UNDO_BUTTON_Y GRID_VAL_TO_COORD(1)
#define UNDO_BUTTON_TXT "Undo" // TODO: add undo character instead

// Functions
#if defined(__APPLE__) && defined(__MACH__)
static bool undo_action_shortcut(void) {
  return (IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER)) &&
         IsKeyPressed(KEY_Z);
}
#else
static bool undo_action_shortcut(void) {
  return (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) &&
         IsKeyPressed(KEY_Z);
}
#endif

void undo_last(Simulation simulation[static 1]) {
  if (simulation->undo_list.len == 0) {
    puts("no more undo actions to perform");
    return;
  }

  UndoProc proc = undo_list_get_last(&simulation->undo_list);
  proc.fn(simulation, &proc);
  undo_list_pop(&simulation->undo_list);
}

static void undo_action_cancel(Simulation _[static 1],
                               Action undo_action[static 1]) {
  undo_action->active = false;
}

static void undo_action_update(Simulation simulation[static 1],
                               Action undo_action[static 1]) {
  if (action_activated(simulation, undo_action)) {
    puts("Undo Action Activated");
    undo_last(simulation);
  }
}

// Button
static Button UNDO_BUTTON = {
    .txt = UNDO_BUTTON_TXT,
    BTN_RECT(UNDO_BUTTON_X, UNDO_BUTTON_Y),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action UNDO_ACTION = {
    .data = NULL,
    .active = false,
    .allow_selection = false,
    .prevent_directly_switching_action = false,
    .button = &UNDO_BUTTON,
    .shortcut_cond = undo_action_shortcut,
    .CANCEL_FN = undo_action_cancel,
    .UPDATE_FN = undo_action_update,
    .RENDER_FN = NULL,
};

Action *undo_action_init(void) {
  // #ifndef NDEBUG
  // action_init(&UNDO_ACTION, 0);
  // #endif
  return &UNDO_ACTION;
}
