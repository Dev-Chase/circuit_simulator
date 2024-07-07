#ifndef SIMULATION_H
#define SIMULATION_H

#include "action.h"
#include "component.h"
#include "component_group.h"
#include "undo_list.h"
#ifdef _cplusplus
extern "C" {
#endif

#define SIMULATION_AREA_X 0
#define SIMULATION_AREA_Y GRID_VAL_TO_COORD(3)
#define SIMULATION_AREA_RECT                                                   \
  RECT(SIMULATION_AREA_X, SIMULATION_AREA_Y, WIDTH, HEIGHT - SIMULATION_AREA_Y)

// Simulation Actions
// #define N_ACTIONS 7
#define N_ACTIONS 6
Action *new_action_init(void);
Action *run_action_init(void);
Action *place_action_init(void);
Action *delete_action_init(void);
Action *select_action_init(void);
Action *undo_action_init(void);
// Action *save_action_init(void);
// Action *import_action_init(void);

typedef struct Simulation {
  Action *actions[N_ACTIONS];
  Component circuit;
  UndoList undo_list;
  size_t hovered_i;
  ComponentGroup hovered;
  ComponentGroup selected;
  ComponentGroup removing;
  bool running; // TODO: implement
} Simulation;

Simulation simulation_new(void);
bool simulation_other_action_prevent_switch(
    const Simulation simulation[static 1], const Action action[static 1]);
size_t simulation_get_hovered(Simulation simulation[static 1]);
bool simulation_should_highlight(const Simulation simulation[static 1]);
void simulation_update(Simulation simulation[static 1]);
void simulation_render(const Simulation simulation[static 1]);
void simulation_free(Simulation simulation[static 1]);

#ifdef _cplusplus
}
#endif
#endif
