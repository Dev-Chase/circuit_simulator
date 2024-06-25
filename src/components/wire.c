#include "wire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Component wire_component(Wire ptr[static 1]) {
  return (Component){
      .ptr = (void *)ptr,

      // Data Manipulation
      .add_data = wire_add_point,
      .del_data = wire_del_point,
      .clear = wire_clear,
      .free = wire_free,

      // Action Functions
      .save = wire_save,
      // .run = wire_run, TODO: implement
      // .place = wire_place, TODO: implement

      // Rendering
      .render = wire_render,
      // .render_run = NULL, TODO: implement
  };
}

#define STARTING_POINTS_CAPACITY 20
Wire wire_new(Vector2 first_point, Color colour) {
  Wire wire = (Wire){
      .points = (Vector2 *)malloc(sizeof(Vector2) * STARTING_POINTS_CAPACITY),
      .points_capacity = STARTING_POINTS_CAPACITY,
      .points_len = 1,
      .colour = colour,
  };
  wire.this_component = wire_component(&wire);
  wire.points[0] = first_point;

  return wire;
}

// Data Manipulation
void wire_add_point(const Component wire_component[static 1],
                    const void *POINT) {
  Wire *wire = (Wire *)wire_component->ptr;
  wire->points_len++;
  wire->points = realloc(wire->points, wire->points_len * sizeof(Vector2));

#ifndef NDEBUG
  if (wire->points == NULL) {
    perror("Failed to realloc!");
    exit(EXIT_FAILURE);
  }
#endif

  wire->points[wire->points_len - 1] = *(Vector2 *)POINT;
}

void wire_del_point(const Component wire_component[static 1], size_t ind) {
  Wire *wire = (Wire *)wire_component->ptr;
  if (ind != wire->points_len - 1) {
    memmove(wire->points + ind, wire->points + (ind + 1),
            (wire->points_len - ind - 1) * sizeof(Vector2));
  }

  wire->points_len--;
  wire->points = realloc(wire->points, sizeof(Vector2) * wire->points_len);
  if (wire->points == NULL) {
    fprintf(stderr, "Failed to Realloc !");
    exit(EXIT_FAILURE);
  }
}

void wire_clear(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  wire->points_len = 0;

#ifndef NDEBUG
  memset(wire->points, 0, wire->points_len * sizeof(wire->points[0]));
#endif
}

void wire_free(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  free(wire->points);

#ifndef NDEBUG
  wire->points = NULL;
  wire->points_len = 0;
  wire->points_capacity = 0;
#endif
}

// save, run, place

// static bool wire_point_in_line(Wire wire[static 1], size_t i) {
// 	return (wire->points[i].x == wire->points[i - 1].x || 
// 	wire->points[i].y == wire->points[i - 1].y);
// }

void wire_render(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  for (size_t i = 1; i < wire->points_len; i++) {
    // if (wire_point_in_lien(wire, i))
    // TODO: draw corners
    // }
    DrawLineEx(wire->points[i], wire->points[i - 1], WIRE_THICKNESS,
               wire->colour);
  }
}

// render_run
//
