#ifndef COMPONENT_I_H
#define COMPONENT_I_H

#include "raylib.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

typedef struct ComponentPoint {
  Vector2 grid_pos;
  float volt;
  float amp;
  float res;
} ComponentPoint;

typedef struct Component Component;
// typedef void (*ComponentRunFn)(Component *self);
// typedef void (*ComponentSaveFn)(Component *self);
// typedef void (*ComponentRenderFn)(Component *const SELF);

typedef struct Component {
  void *ptr;

  // Data Manipulation
  void (*add_data)(const Component *self, const void *const DATA);
  void (*del_data)(const Component *self, size_t ind);
  void (*clear)(const Component *self);
  void (*free)(Component *self);

  // Action Functions
  void (*save)(const Component *const SELF, const char *path);
  void (*run)(const Component *self); // sets ends
  void (*place)(Component *self, Vector2 grid_pos);

  // Rendering
  void (*render)(const Component *const SELF);
  void (*render_highlight)(const Component *const SELF);
  void (*render_run)(const Component *const SELF);

  // Information
  bool (*is_hovered)(const Component *const SELF);

  ComponentPoint *ends;
} Component;

#ifndef NDEBUG
#define COMPONENT_FN(component, fn)                                            \
  if (component.fn != NULL) {                                                  \
    component.fn(&component);                                                  \
  }
#define COMPONENT_FN_PARAMS(component, fn, params)                             \
  if (component.fn != NULL) {                                                  \
    component.fn(&component, params);                                          \
  }

// NOTE: the component must implement the returning function, otherwise UB
#define COMPONENT_FN_RETURNS(component, fn) component.fn(&component)
#define COMPONENT_FN_RETURNS_PARAMS(component, fn, params)                     \
  component.fn(&component, params)
#else
#define COMPONENT_FN(component, fn) component->fn(component)
#define COMPONENT_FN_PARAMS(component, fn, params)                             \
  component->fn(component, params)
#endif

#ifdef _cplusplus
}
#endif
#endif
