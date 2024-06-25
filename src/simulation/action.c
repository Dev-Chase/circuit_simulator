#include "action.h"
#include "button.h"
#include "simulation.h"
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

bool action_activated(Action action[static 1]) {
  bool button_event =
      (action->button) ? button_is_clicked(action->button) : false;
  bool shortcut_event =
      (action->shortcut_cond) ? action->shortcut_cond() : false;

  return button_event || shortcut_event;
}

void action_update(Simulation *simulation, Action action[static 1]) {
  if (action->UPDATE_FN) {
    action->UPDATE_FN(simulation, action);
  }
}

void action_render(const Action action[static 1]) {
  if (action->button && action->button->txt) {
    button_render(action->button);
  }

  if (action->RENDER_FN) {
    action->RENDER_FN(action);
  }
}

void action_free(Action action[static 1]) {
  if (action->data != NULL) {
    free(action->data);
  }
}
