// TODO: implement a way to save circuit to file
#ifndef BATTERY_H
#define BATTERY_H

#include "component.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif

// Render Information
#define BATTERY_LINE_WIDTH 4
#define BATTERY_COLOUR FG_COLOUR

typedef struct Battery {
  int voltage;
  Rectangle rect;
  bool is_horizontal;
} Battery;

Component
battery_new(int voltage,
            Rectangle rect); // TODO: perhaps add advanced specifications

/** Component Interface */
// Data Manipulation
#define BATTERY_ADD NULL
#define BATTERY_DEL NULL
#define BATTERY_CLEAR NULL
#define BATTERY_FREE NULL

// Action Functions
void battery_save(const Component battery_component[static 1],
                  const char *path);
void battery_run(Component battery_component[static 1]);
void battery_place(Component battery_component[static 1], Vector2 grid_pos);

// Rendering
void battery_render(const Component battery_component[static 1]);
void battery_render_run(const Component battery_component[static 1]);

// Information
bool battery_is_hovered(const Component battery_component[static 1]);
bool battery_collides_rect(const Component battery_component[static 1],
                           Rectangle rect);

#ifdef _cplusplus
}
#endif
#endif
