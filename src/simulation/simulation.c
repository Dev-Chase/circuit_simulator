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
            },
        .active_component = calloc(1, sizeof(Component)),
        .circuit = {0},
        .running = false,
    };
    circuit_fill_new(&simulation.circuit, NULL);
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

  for (size_t i = 0; i < simulation->circuit.components_len; i++) {
    COMPONENT_FN(simulation->circuit.components[i], render)
  }

  // TODO: switch with open circuit render in order to properly seperate circuit
  // component function and simulation render function
  COMPONENT_FN(simulation->circuit.this_component, render)
}

void simulation_free(Simulation simulation[static 1]) {
  free(simulation->active_component);
#ifndef NDEBUG
  simulation->active_component = NULL;
#endif

  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_free(simulation->actions[i]);
  }
}
