#include "simulation.h"
#include "action.h"
#include "circuit.h"
#include "component.h"
#include "stddef.h"
#include "utils.h"
#include <limits.h>
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
            },
        .active_component = {0},
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

bool simulation_other_action_active(Simulation simulation[static 1],
                                    Action action[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    if (simulation->actions[i]->active && simulation->actions[i] != action) {
      return true;
    }
  }

  return false;
}

void simulation_update(Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_update(simulation, simulation->actions[i]);
  }
}

void simulation_render(const Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_render(simulation, simulation->actions[i]);
  }

  Circuit *circuit = (Circuit *)simulation->circuit.ptr;
  for (size_t i = 0; i < circuit->components_len; i++) {
    COMPONENT_FN(circuit->components[i], render)
  }
  COMPONENT_FN(simulation->active_component, render)
}

size_t simulation_get_hovered_component(const Simulation simulation[static 1]) {
  Circuit *circuit = simulation->circuit.ptr;

  for (size_t i = 0; i < circuit->components_len; i++) {
    if (COMPONENT_FN_RETURNS(circuit->components[i], is_hovered)) {
      return i;
    }
  }

  return SIZE_T_MAX;
}

void simulation_free(Simulation simulation[static 1]) {
  COMPONENT_FN(simulation->circuit, free)
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_free(simulation->actions[i]);
  }
}
