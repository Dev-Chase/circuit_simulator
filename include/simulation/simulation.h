#ifndef SIMULATION_H
#define SIMULATION_H

#include "action.h"
#include "circuit.h"
#ifdef _cplusplus
extern "C" {
#endif

// Simulation Actions
// #define N_ACTIONS 5
#define N_ACTIONS 3
Action *new_action_init(void);
Action *run_action_init(void);
// Action *import_action_init(void);
Action *place_action_init(void);
// Action *save_action_init(void);

typedef struct Simulation {
  Action *actions[N_ACTIONS];
  Component circuit;
  Component active_component;
  bool running; //TODO: implement
} Simulation;

Simulation simulation_new(void);
void simulation_update(Simulation simulation[static 1]);
void simulation_render(const Simulation simulation[static 1]);
void simulation_free(Simulation simulation[static 1]);

#ifdef _cplusplus
}
#endif
#endif
