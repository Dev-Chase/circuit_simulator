#ifndef ACTION_H
#define ACTION_H

#include "button.h"
#include <stdbool.h>
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

typedef struct Simulation Simulation;
typedef struct Action Action;

typedef bool (*ShortcutConditionFn)(void);
typedef void (*ActionCancelFn)(Simulation *, Action *);
typedef void (*ActionUpdateFn)(Simulation *, Action *);
typedef void (*ActionRenderFn)(const Simulation *const, const Action *const);

struct Action {
  void *data;
  bool active;
  bool allow_selection;
  bool prevent_directly_switching_action;
  Button *button;
  ShortcutConditionFn shortcut_cond;
  const ActionCancelFn CANCEL_FN;
  const ActionUpdateFn UPDATE_FN;
  const ActionRenderFn RENDER_FN;
};

void action_init(Action action[static 1], size_t data_size);
bool action_activated(Simulation *simulation, Action action[static 1]);
void action_update(Simulation *simulation, Action action[static 1]);
void action_render(const Simulation *SIMULATION, const Action action[static 1]);
void action_free(Action action[static 1]);
void action_activate(Simulation *simulation, Action action[static 1]);
void action_toggle(Simulation *simulation, Action action[static 1]);

#ifdef _cplusplus
}
#endif
#endif
