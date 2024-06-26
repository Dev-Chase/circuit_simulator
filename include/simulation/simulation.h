#ifndef SIMULATION_H
#define SIMULATION_H

#include "action.h"
#include "component.h"
#ifdef _cplusplus
extern "C" {
#endif

// Simulation Actions
// #define N_ACTIONS 5
#define N_ACTIONS 4
Action *new_action_init(void);
Action *run_action_init(void);
// Action *import_action_init(void);
Action *place_action_init(void);
Action *delete_action_init(void);
// Action *save_action_init(void);

typedef struct Simulation {
  Action *actions[N_ACTIONS];
  Component circuit;
  Component active_component;
  bool running; // TODO: implement
} Simulation;

Simulation simulation_new(void);
bool simulation_other_action_active(Simulation simulation[static 1],
                                    Action action[static 1]);
void simulation_update(Simulation simulation[static 1]);
void simulation_render(const Simulation simulation[static 1]);
size_t simulation_get_hovered_component(
    const Simulation
        simulation[static 1]); // TODO: implement (highlight hovered component)
void simulation_free(Simulation simulation[static 1]);

#ifdef _cplusplus
}
#endif
#endif
