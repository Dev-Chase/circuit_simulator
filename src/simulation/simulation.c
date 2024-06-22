#include "simulation.h"
#include "action.h"
#include "circuit.h"
#include "component.h"
#include "stddef.h"
#include <stdio.h>
#include <string.h>

Simulation simulation_new(void) {
  static Simulation simulation = {0};
  if (!memcmp(&simulation, &(Simulation){0}, sizeof(Simulation))) {
    simulation = (Simulation){
        .actions =
            {
                new_action_init(),
                run_action_init(),
            },
        .circuit = {0},
        .running = false,
    };
    circuit_fill_new(&simulation.circuit, NULL);
  } else {
    perror("calling simulation_new twice whilst Simulation is supposed to "
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

  COMPONENT_FN(simulation->circuit.this_component, render)
}

void simulation_free(Simulation simulation[static 1]) {
  for (size_t i = 0; i < N_ACTIONS; i++) {
    action_free(simulation->actions[i]);
  }
}
