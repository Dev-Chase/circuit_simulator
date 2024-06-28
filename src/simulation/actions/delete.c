#include "action.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <stdio.h>

#define DEL_X GRID_VAL_TO_COORD(13)
#define DEL_Y GRID_VAL_TO_COORD(1)
#define DEL_TXT "Delete"

// Functions
static bool del_action_shorcut(void) {
  return IsKeyPressed(KEY_D) &&
         (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT));
}

//TODO:: add activated action state
static void del_action_update(Simulation _[static 1],
                              Action del_action[static 1]) {
  if (action_activated(del_action)) {
    *(bool *)del_action->data = !*(bool *)del_action->data;
    printf("Activated Delete Action!, Data: %hhu\n", *(bool *)del_action->data);
  }

  if (IsKeyPressed(KEY_ESCAPE) && *(bool *)del_action->data) {
    *(bool *)del_action->data = false;
    puts("Cancelled Deletion");
  }
  // TODO: implement
}

static void del_action_render(const Action del_action[static 1]) {
  if (*(bool *)del_action->data) {
    DrawRectangle(GetMouseX() - 10, GetMouseY() - 10, 20, 20, FG_COLOUR);
  }
}

static Button DEL_BUTTON = {
    .txt = DEL_TXT,
    .RECT = BTN_RECT(DEL_X, DEL_Y),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action DEL_ACTION = {
    .data = NULL,
    .button = &DEL_BUTTON,
    .shortcut_cond = del_action_shorcut,
    .UPDATE_FN = del_action_update,
    .RENDER_FN = del_action_render,
};

Action *delete_action_init(void) {
  action_init(&DEL_ACTION, sizeof(bool));
  *(bool *)DEL_ACTION.data = false;

  return &DEL_ACTION;
}
