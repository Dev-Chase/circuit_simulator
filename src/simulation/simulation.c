#include "simulation.h"
#include "action.h"
#include "circuit.h"
#include "component.h"
#include "component_group.h"
#include "stddef.h"
#include <limits.h>
#include <machine/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Simulation simulation_new(void) {
  static Simulation simulation = {0};
  if (!memcmp(&simulation, &(Simulation){0}, sizeof(Simulation))) {
    simulation = (Simulation){
        .actions =
            {
                new_action_init(),
                run_action_init(),
                place_action_init(),
                delete_action_init(),
                select_action_init(),
            },
        // .selected_component = {0},
        .selected = component_group_new(),
        .hovered = component_group_new(),
        .hovered_i = SIZE_T_MAX,
        .circuit = circuit_new(NULL, 0, 0),
        .running = false,
    };
  } else {
    perror("initialized Simulation type (simulation_new) twice whilst "
           "Simulation is supposed to "
           "be a singleton");
  }

  return simulation;
}

bool simulation_other_action_prevent_switch(
    const Simulation simulation[static 1], const Action *action) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    if (simulation->actions[i]->active &&
        simulation->actions[i]->prevent_directly_switching_action &&
        simulation->actions[i] != action) {
      return true;
    }
  }

  return false;
}

size_t simulation_get_hovered(Simulation simulation[static 1]) {
  Circuit *sim_circuit = (Circuit *)(simulation->circuit.ptr);

  for (size_t i = 0; i < sim_circuit->components_len; i++) {
    if (COMPONENT_FN_RETURNS(sim_circuit->components[i], is_hovered)) {
      return i;
    }
  }

  return SIZE_T_MAX;
}

// TODO: implement multiple selection
static bool simulation_should_deselect(const Simulation simulation[static 1]) {
  bool any_hovered = simulation->hovered.component_len != 0;
  bool valid_click =
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                             SIMULATION_AREA_RECT);
  bool is_shift_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

  return ((!any_hovered && valid_click) || IsKeyPressed(KEY_ESCAPE) ||
          (valid_click && any_hovered && !is_shift_down)) &&
         simulation->selected.component_len != 0;
}

bool simulation_should_highlight(const Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    if (simulation->actions[i]->active) {
      if (simulation->actions[i]->allow_selection) {
        break;
      }
      return false;
    }
  }

  return true;
  // return simulation->hovered.component_len != 0;
}

void simulation_update(Simulation simulation[static 1]) {
  simulation->hovered_i = simulation_get_hovered(simulation);
  // component_group_clear(&simulation->hovered);
  // if (simulation->hovered_i != SIZE_T_MAX) {
  //   component_group_add(&simulation->hovered, simulation->hovered_i);
  // }

  if (simulation_should_deselect(simulation)) {
    component_group_clear(&simulation->selected);
  }

  // if (simulation_should_highlight(simulation) &&
  //     (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
  //      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
  //                             SIMULATION_AREA_RECT))) {
  //   puts("Selected");
  //   component_group_add(&simulation->selected, simulation->hovered_i);
  // }

  // Update Actions
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_update(simulation, simulation->actions[i]);
  }
}

void simulation_highlight_selected(const Simulation simulation[static 1]) {
  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;
  for (size_t i = 0; i < simulation->selected.component_len; i++) {
    COMPONENT_FN(sim_circuit->components[simulation->selected.component_is[i]],
                 render_highlight)
  }
}

void simulation_render(const Simulation simulation[static 1]) {
  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;

  // Highlighting
  if (simulation_should_highlight(simulation)) {
    for (size_t i = 0; i < simulation->hovered.component_len; i++) {
      COMPONENT_FN(sim_circuit->components[simulation->hovered.component_is[i]],
                   render_highlight)
    }
    simulation_highlight_selected(simulation);
  }

  // Component Drawing
  for (size_t i = 0; i < sim_circuit->components_len; i++) {
    COMPONENT_FN(sim_circuit->components[i], render)
  }

  // Render Actions
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_render(simulation, simulation->actions[i]);
  }
}

void simulation_free(Simulation simulation[static 1]) {
  COMPONENT_FN(simulation->circuit, free)
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_free(simulation->actions[i]);
  }
}
