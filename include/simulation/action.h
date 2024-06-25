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
typedef void (*ActionRenderFn)(const Action *const);
typedef void (*ActionUpdateFn)(Simulation *, Action *);

struct Action {
  void *data;
  Button *button;
  ShortcutConditionFn shortcut_cond;
  const ActionRenderFn RENDER_FN;
  const ActionUpdateFn UPDATE_FN;
};

void action_init(Action action[static 1], size_t data_size);
bool action_activated(Action action[static 1]);
void action_update(Simulation *simulation, Action action[static 1]);
void action_render(const Action action[static 1]);
void action_free(Action action[static 1]);

#ifdef _cplusplus
}
#endif
#endif
