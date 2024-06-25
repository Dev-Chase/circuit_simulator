// TODO: implement a way to save circuit to file
#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "component.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

typedef struct Circuit {
  const char *path;
  Component this_component;
  Component *components;
  void *component_data; // NOTE: stored as data_size, then data
  size_t component_data_size;
  size_t components_len;
  size_t components_capacity;
  Vector2 pos;
  //TODO: add draw data and implement rendering for imported circuits vs open circuit
} Circuit;

void circuit_fill_new(Circuit circuit[static 1], const char *path);

/** Component Interface */
// Data Manipulation
void circuit_add_component(const Component circuit_component[static 1],
                           const void *COMPONENT, size_t size);
void circuit_del_component(const Component circuit_component[static 1],
                           size_t ind);
void circuit_clear(const Component circuit_component[static 1]);
void circuit_free(const Component circuit_component[static 1]);

// Action Functions
void circuit_save(const Component circuit_component[static 1],
                  const char *path);
void circuit_run(const Component circuit_component[static 1]);
void circuit_place(Component circuit_component[static 1], Vector2 grid_pos);

// Rendering
void circuit_render(const Component circuit_component[static 1]);
void circuit_render_run(const Component circuit_component[static 1]);

#ifdef _cplusplus
}
#endif
#endif
