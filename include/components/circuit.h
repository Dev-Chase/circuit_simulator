// TODO: implement a way to save circuit to file
#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "component.h"
#include "component_group.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

typedef struct Circuit {
  const char *path;
  Component *components;
  size_t components_len;
  size_t components_capacity; // in number of components
  Rectangle rect;
  // TODO: add draw data and implement rendering for imported circuits vs open
  // circuit
} Circuit;

Component circuit_new(const char *path, float tile_width, float tile_height);

/** Component Interface */
// Data Manipulation
void circuit_add_component(const Component circuit_component[static 1],
                           const void *COMPONENT);
void circuit_del_component(const Component circuit_component[static 1],
                           size_t ind);
void circuit_del_components(const Component circuit_component[static 1],
                            ComponentGroup components[static 1]);
void circuit_clear(const Component circuit_component[static 1]);
void circuit_free(Component circuit_component[static 1]);

// Action Functions
void circuit_save(const Component circuit_component[static 1],
                  const char *path);
void circuit_run(const Component circuit_component[static 1]);
void circuit_place(Component circuit_component[static 1], Vector2 grid_pos);

// Rendering
void circuit_render(const Component circuit_component[static 1]);
void circuit_render_run(const Component circuit_component[static 1]);

// Information
bool circuit_is_hovered(const Component circuit_component[static 1]);
bool circuit_collides_rect(const Component circuit_component[static 1],
                           Rectangle rect);

#ifdef _cplusplus
}
#endif
#endif
