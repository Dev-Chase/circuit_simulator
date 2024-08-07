#ifndef WIRE_H
#define WIRE_H

#include "component.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

#define WIRE_THICKNESS 2
#define WIRE_HIGHLIGHTED_THICKNESS WIRE_THICKNESS * 2
#define WIRE_HIGHLIGHTED_COLOUR WHITE
#define WIRE_COLOUR RED
#define WIRE_END_COLOUR                                                        \
  (Color) { 114, 114, 114, 255 }

typedef struct Wire {
  Vector2 *points;
  size_t points_capacity;
  size_t points_len;
  bool is_last_set;
  Color colour;
  // TODO: add draw data and implement rendering for imported circuits vs open
  // circuit
} Wire;

// Initialization
Component wire_new(Color colour);

// Utlity Functions
size_t wire_last_i(const Wire wire[static 1]);
bool wire_contains_point(Wire wire[static 1], Vector2 point,
                         bool last_point_set);

/** Component Interface */
// Data Manipulation
void wire_add_point(const Component wire_component[static 1],
                    const void *POINT);
void wire_set_last(Wire wire[static 1], Vector2 point);
void wire_del_point(const Component wire_component[static 1], size_t ind);
void wire_clear(const Component wire_component[static 1]);
void wire_free(Component wire_component[static 1]);

// Action Functions
void wire_save(const Component wire_component[static 1], const char *path);
void wire_run(Component wire_component[static 1]);
void wire_place(Component wire_component[static 1], Vector2 _);

// Rendering
void wire_render(const Component wire_component[static 1]);
void wire_render_highlight(const Component wire_component[static 1]);
void wire_render_run(const Component wire_component[static 1]);

// Information
bool wire_is_hovered(const Component wire_component[static 1]);
bool wire_collides_rect(const Component wire_component[static 1],
                        Rectangle rect);

#ifdef _cplusplus
}
#endif
#endif
