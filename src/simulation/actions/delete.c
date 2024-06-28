#include "action.h"
#include "circuit.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>

#define DEL_X GRID_VAL_TO_COORD(13)
#define DEL_Y GRID_VAL_TO_COORD(1)
#define DEL_TXT "Delete"

// Functions
static bool del_action_shorcut(void) {
  return IsKeyPressed(KEY_D) &&
         (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT));
}

// TODO:: add activated action state
static void del_action_update(Simulation simulation[static 1],
                              Action del_action[static 1]) {

  if (action_activated(simulation, del_action)) {
    del_action->active = !del_action->active;
    printf("Activated Delete Action!, Data: %hhu\n", del_action->active);
  }

  if (del_action->active) {
    size_t hovered_i = simulation_get_hovered_component(simulation);
    *(size_t *)del_action->data = hovered_i;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hovered_i != SIZE_T_MAX) {
      circuit_del_component(&simulation->circuit, hovered_i);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      del_action->active = false;
      puts("Cancelled Deletion");
    }
  }
  // TODO: implement
}

static void del_action_render(const Simulation simulation[static 1],
                              const Action del_action[static 1]) {
  if (del_action->active) {
    size_t hovered_i = *(size_t *)del_action->data;

    if (hovered_i != SIZE_T_MAX) {
      Circuit *circuit = (Circuit *)simulation->circuit.ptr;

      COMPONENT_FN(circuit->components[hovered_i], render_highlight);
    }

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
    .active = false,
    .button = &DEL_BUTTON,
    .shortcut_cond = del_action_shorcut,
    .UPDATE_FN = del_action_update,
    .RENDER_FN = del_action_render,
};

Action *delete_action_init(void) {
  // Initializing
  action_init(&DEL_ACTION, sizeof(size_t));
  *(size_t *)DEL_ACTION.data = SIZE_T_MAX;

  return &DEL_ACTION;
}
