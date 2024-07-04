#include "action.h"
#include "button.h"
#include "simulation.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void action_init(Action action[static 1], size_t data_size) {
  if (action->data != NULL) {
    perror("Called Action initialize twice !");
    exit(EXIT_FAILURE);
  }

#ifndef NDEBUG
  action->data = (data_size) ? malloc(data_size) : NULL;
#else
  action->data = malloc(data_size);
#endif
}

bool action_activated(Simulation simulation[static 1],
                      Action action[static 1]) {
  bool button_event =
      (action->button) ? button_is_clicked(action->button) : false;
  bool shortcut_event =
      (action->shortcut_cond) ? action->shortcut_cond() : false;

  return (button_event || shortcut_event) &&
         !simulation_other_action_prevent_switch(simulation, action);
}

void action_update(Simulation *simulation, Action action[static 1]) {
  if (action->UPDATE_FN) {
    action->UPDATE_FN(simulation, action);
  }
}

void action_render(const Simulation *const SIMULATION,
                   const Action action[static 1]) {
  if (action->button && action->button->txt) {
    button_render(action->button);
  }

  if (action->RENDER_FN) {
    action->RENDER_FN(SIMULATION, action);
  }
}

void action_free(Action action[static 1]) {
  if (action->data != NULL) {
    free(action->data);
  }
}

void action_activate(Simulation *simulation, Action action[static 1]) {
  size_t i;
  for (i = 0; i < N_ACTIONS; i++) {
    if (simulation->actions[i]->active) {
      assert(action->CANCEL_FN != NULL);
      simulation->actions[i]->CANCEL_FN(simulation, simulation->actions[i]);
    }
  }

  action->active = true;
}

void action_toggle(Simulation *simulation, Action action[static 1]) {
  if (action->active) {
    assert(action->CANCEL_FN != NULL);
    action->CANCEL_FN(simulation, action);
  } else {
    action_activate(simulation, action);
  }
}
