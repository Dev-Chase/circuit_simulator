#include "action.h"
#include "circuit.h"
#include "component.h"
#include "component_group.h"
#include "raylib.h"
#include "simulation.h"
#include "undo_list.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEL_X GRID_VAL_TO_COORD(13)
#define DEL_Y GRID_VAL_TO_COORD(1)
#define DEL_TXT "Delete"

// Functions
static bool del_action_shortcut(void) { return IsKeyPressed(KEY_D); }

// TODO: find error
void del_action_undo_expired(UndoProc proc[static 1]) {
  size_t component_len = proc->size / sizeof(Component);
  Component *components = (Component *)proc->data;
  for (size_t i = 0; i < component_len; i++) {
    COMPONENT_FN(components[i], free)
  }
  free(components);
#ifndef NDEBUG
  memset(proc, 0, sizeof(UndoProc));
#endif
}

void del_action_undo(Simulation simulation[static 1], UndoProc proc[static 1]) {
  size_t component_len = proc->size / sizeof(Component);
  Component *components = (Component *)proc->data;
  for (size_t i = 0; i < component_len; i++) {
    circuit_append_component(&simulation->circuit, (void *)&(components[i]));
  }
}

static void del_action_create_undo(Simulation simulation[static 1]) {
  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;

  size_t data_size = simulation->selected.len * sizeof(Component);
  Component *data = (Component *)malloc(data_size);
  for (size_t i = 0; i < simulation->selected.len; i++) {
    size_t component_i =
        circuit_get_i_from_id(sim_circuit, simulation->selected.ids[i]);
    data[i] = sim_circuit->components[component_i];
  }

  undo_list_append(&simulation->undo_list,
                   (UndoProc){
                       .fn = del_action_undo,
                       .expired_fn = del_action_undo_expired,
                       .data = data,
                       .size = data_size,
                   });
}

static void del_selected(Simulation simulation[static 1]) {
  assert(simulation->selected.len != 0);

  del_action_create_undo(simulation);
  circuit_remove_group(&simulation->circuit, &simulation->selected);
  component_group_clear(&simulation->selected);
}
static void del_action_cancel(Simulation _[static 1],
                              Action del_action[static 1]) {
  del_action->active = false;
  puts("Finished Deletion");
}

static void del_action_update(Simulation simulation[static 1],
                              Action del_action[static 1]) {

  if (action_activated(simulation, del_action)) {
    action_toggle(simulation, del_action);
  }

  if (IsKeyPressed(KEY_BACKSPACE)) {
    if (simulation->selected.len != 0) {
      del_selected(simulation);
    }
  }

  if (del_action->active) {
    if (simulation->selected.len != 0) {
      del_selected(simulation);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      del_action_cancel(simulation, del_action);
    }
  }
}

static void del_action_render(const Simulation _[static 1],
                              const Action del_action[static 1]) {
  if (del_action->active) {
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
    .allow_selection = true,
    .button = &DEL_BUTTON,
    .shortcut_cond = del_action_shortcut,
    .CANCEL_FN = del_action_cancel,
    .UPDATE_FN = del_action_update,
    .RENDER_FN = del_action_render,
};

Action *delete_action_init(void) {
  // Initializing
  // action_init(&DEL_ACTION, sizeof(size_t));
  // *(size_t *)DEL_ACTION.data = SIZE_T_MAX;

  return &DEL_ACTION;
}
