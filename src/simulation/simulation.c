#include "simulation.h"
#include "action.h"
#include "circuit.h"
#include "component.h"
#include "stddef.h"
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
        .circuit = circuit_new(NULL),
        .running = false,
    };
  } else {
    perror("initialized Simulation type (simulation_new) twice whilst "
           "Simulation is supposed to "
           "be a singleton");
  }

  return simulation;
}

void simulation_update(Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_update(simulation, simulation->actions[i]);
  }
}

void simulation_render(const Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_render(simulation->actions[i]);
  }

  Circuit *circuit = (Circuit *)simulation->circuit.ptr;
  for (size_t i = 0; i < circuit->components_len; i++) {
    COMPONENT_FN(circuit->components[i], render)
  }
  COMPONENT_FN(simulation->active_component, render)
}

void simulation_free(Simulation simulation[static 1]) {
  COMPONENT_FN(simulation->circuit, free)
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_free(simulation->actions[i]);
  }
}
