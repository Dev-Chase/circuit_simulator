#include "wire.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_POINTS_CAPACITY 20
Component wire_new(Color colour) {
  Component component = (Component){
      .ptr = malloc(sizeof(Wire)),

      // Data Manipulation
      .add_data = wire_add_point,
      .del_data = wire_del_point,
      .clear = wire_clear,
      .free = wire_free,

      // Action Functions
      // .save = wire_save,//TODO: implement
      // .run = wire_run, TODO: implement
      // .place = wire_place, TODO: implement

      // Rendering
      .render = wire_render,
      .render_highlight = wire_render_highlight,
      // .render_run = NULL, TODO: implement

      // Information
      .is_hovered = wire_is_hovered,
  };

  Wire *wire = (Wire *)component.ptr;
  *wire = (Wire){
      .points = (Vector2 *)malloc(sizeof(Vector2) * STARTING_POINTS_CAPACITY),
      .points_capacity = STARTING_POINTS_CAPACITY,
      .is_last_set = false,
      .points_len = 0,
      .colour = colour,
  };

  return component;
}

size_t wire_last_i(const Wire wire[static 1]) {
  return (wire->is_last_set) ? wire->points_len - 2 : wire->points_len - 1;
}

bool points_in_line(Vector2 point_a, Vector2 point_b) {
  return (point_a.x == point_b.x || point_a.y == point_b.y);
}

bool wire_contains_point(Wire wire[static 1], Vector2 point,
                         bool last_point_set) {
  if (wire->points_len == 0) {
    return false;
  }
  size_t points_limit =
      last_point_set ? wire->points_len - 1 : wire->points_len;
  for (size_t i = 0; i < points_limit; i++) {
    if (wire->points[i].x == point.x && wire->points[i].y == point.y) {
      return true;
    }
  }

  return false;
}

// Data Manipulation
void wire_add_point(const Component wire_component[static 1],
                    const void *POINT) {
  Wire *wire = (Wire *)wire_component->ptr;
  // TODO: implmenet corners here

  if (wire->points_len + 1 > wire->points_capacity) {
    wire->points_capacity *= 2;
    wire->points =
        realloc(wire->points, wire->points_capacity * sizeof(Vector2));

#ifndef NDEBUG
    if (wire->points == NULL) {
      perror("Failed to realloc!");
      exit(EXIT_FAILURE);
    }
#endif
  }

  size_t last_i = wire_last_i(wire);
  Vector2 point_a = wire->points[last_i];
  Vector2 point_b = *(Vector2 *)POINT;

  // TODO: change drawing corners like this to be put when creating wires
  if (!points_in_line(point_a, point_b)) {
    Vector2 point_c;
    if (fabsf(point_a.x - point_b.x) > fabsf(point_a.y - point_b.y) &&
        wire_contains_point(wire, VEC2(point_b.x, point_a.y), false)) {
      point_c = (Vector2){point_b.x, point_a.y};
    } else {
      point_c = (Vector2){point_a.x, point_b.y};
    }
    wire_add_point(wire_component, &point_c);
  }

  wire->points[wire->points_len - 1] = *(Vector2 *)POINT;
  wire->points_len++;
}

void wire_set_last(Wire wire[static 1], Vector2 point) {
  wire->points[wire->points_len - 1] = point;
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

void wire_free(Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  free(wire->points);

#ifndef NDEBUG
  wire->points = NULL;
  wire->points_len = 0;
  wire->points_capacity = 0;
#endif
}

// save, run, place

void wire_render_segment(Wire wire[static 1], size_t i, int thickness) {
  Vector2 point_a = wire->points[i - 1];
  Vector2 point_b = wire->points[i];

  if (!points_in_line(point_a, point_b)) {
    Vector2 point_c;
    if (fabsf(point_a.x - point_b.x) > fabsf(point_a.y - point_b.y) &&
        wire_contains_point(wire, VEC2(point_b.x, point_a.y), false)) {
      point_c = (Vector2){point_b.x, point_a.y};
    } else {
      point_c = (Vector2){point_a.x, point_b.y};
    }
    DrawLineEx(point_a, point_c, thickness, wire->colour);
    point_a = point_c;
  }

  DrawLineEx(point_a, point_b, thickness, wire->colour);
}

void wire_render(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;

  for (size_t i = 1; i < wire->points_len; i++) {
    wire_render_segment(wire, i, WIRE_THICKNESS);
  }
}

void wire_render_highlight(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  Color old_colour = wire->colour;
  wire->colour = WIRE_HIGHLIGHTED_COLOUR;

  for (size_t i = 1; i < wire->points_len; i++) {
    wire_render_segment(wire, i, WIRE_HIGHLIGHTED_THICKNESS);
  }

  wire->colour = old_colour;
}

// render_run

static bool is_val_between_vals(float val, float val_a, float val_b) {
  return val >= fminf(val_a, val_b) && val <= fmaxf(val_a, val_b);
}

bool wire_is_hovered(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  for (size_t i = 1; i < wire->points_len; i++) {
    if (CheckCollisionPointLine(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                                wire->points[i - 1], wire->points[i],
                                GRID_VAL_TO_COORD(0.75))) {
      return true;
    }
  }

  return false;
}
