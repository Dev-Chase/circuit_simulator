#include "wire.h"
#include "component_io_pins.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
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
      .place = wire_place,

      // Rendering
      .render = wire_render,
      .render_highlight = wire_render_highlight,
      // .render_run = NULL, TODO: implement

      // Information
      .is_hovered = wire_is_hovered,
      .collides_rect = wire_collides_rect,

      // IOPins
      .pins = component_io_pins_new(),
  };
  uuid_generate_random(component.id);

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
  assert(wire->points_len != 0);
  return (wire->is_last_set) ? wire->points_len - 2 : wire->points_len - 1;
}

bool points_in_line(Vector2 point_a, Vector2 point_b) {
  return point_a.x == point_b.x || point_a.y == point_b.y;
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
  // TODO: implement corners here

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

  if (wire->points_len > 0) {
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
  }

  wire->points[wire->points_len] = *(Vector2 *)POINT;
  wire->points_len++;
}

void wire_set_last(Wire wire[static 1], Vector2 point) {
  wire->points[wire->points_len - 1] =
      vector2_constrain_to_simulation_area(point);
}

void wire_del_point(const Component wire_component[static 1], size_t ind) {
  Wire *wire = (Wire *)wire_component->ptr;
  if (ind != wire->points_len - 1) {
    memmove(wire->points + ind, wire->points + (ind + 1),
            (wire->points_len - ind - 1) * sizeof(Vector2));
  }

  wire->points_len--;

#ifndef NDEBUG
  memset(&wire->points[wire->points_len], 0, sizeof(Vector2));
#endif
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
  free(wire);
}

// save, run

void wire_place(Component wire_component[static 1], Vector2 _) {
  Wire *wire = (Wire *)wire_component->ptr;
  component_io_pins_add_position(&wire_component->pins, wire->points[0]);
  component_io_pins_add_position(&wire_component->pins,
                                 wire->points[wire->points_len - 1]);
}

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
  if (wire_component->pins.pin_len == 2) {
    DrawCircleV(wire_component->pins.pins[0].grid_pos, WIRE_THICKNESS,
                WIRE_END_COLOUR);
    DrawCircleV(wire_component->pins.pins[1].grid_pos, WIRE_THICKNESS,
                WIRE_END_COLOUR);
  }
}

Vector2 get_extended_point(Vector2 first, Vector2 second) {
#define EXTENSION 2
  Vector2 point = first;

  if (first.x == second.x) {
    point.y += copysignf(EXTENSION, first.y - second.y);
  } else {
    assert(first.y == second.y);
    point.x += copysignf(EXTENSION, first.x - second.x);
  }

  return point;
}

void wire_render_highlight(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  Color old_colour = wire->colour;
  Vector2 old_start = wire->points[0];
  Vector2 old_end = wire->points[wire->points_len - 1];

  wire->colour = WIRE_HIGHLIGHTED_COLOUR;
  if (wire->points_len > 1) {
    wire->points[0] = get_extended_point(wire->points[0], wire->points[1]);
    wire->points[wire->points_len - 1] = get_extended_point(
        wire->points[wire->points_len - 1], wire->points[wire->points_len - 2]);
  }

  for (size_t i = 1; i < wire->points_len; i++) {
    wire_render_segment(wire, i, WIRE_HIGHLIGHTED_THICKNESS);
  }

  wire->colour = old_colour;
  if (wire->points_len > 1) {
    wire->points[0] = old_start;
    wire->points[wire->points_len - 1] = old_end;
  }
}

// render_run

bool wire_is_hovered(const Component wire_component[static 1]) {
  Wire *wire = (Wire *)wire_component->ptr;
  for (size_t i = 1; i < wire->points_len; i++) {
    if (CheckCollisionPointLine(GetMousePosition(), wire->points[i - 1],
                                wire->points[i], GRID_VAL_TO_COORD(0.75))) {
      return true;
    }
  }

  return false;
}

static bool line_collides_rect(Vector2 point_a, Vector2 point_b,
                               Rectangle rect) {
  Vector2 top[2] = {VEC2(rect.x, rect.y), VEC2(rect.x + rect.width, rect.y)};
  Vector2 bottom[2] = {VEC2(rect.x, rect.y + rect.height),
                       VEC2(rect.x + rect.width, rect.y + rect.height)};
  Vector2 left[2] = {VEC2(rect.x, rect.y), VEC2(rect.x, rect.y + rect.height)};
  Vector2 right[2] = {VEC2(rect.x + rect.width, rect.y),
                      VEC2(rect.x + rect.width, rect.y + rect.height)};
  if (CheckCollisionLines(point_a, point_b, top[0], top[1], NULL)) {
    return true;
  }
  if (CheckCollisionLines(point_a, point_b, bottom[0], bottom[1], NULL)) {
    return true;
  }
  if (CheckCollisionLines(point_a, point_b, left[0], left[1], NULL)) {
    return true;
  }
  if (CheckCollisionLines(point_a, point_b, right[0], right[1], NULL)) {
    return true;
  }

  if (CheckCollisionPointRec(point_a, rect) ||
      CheckCollisionPointRec(point_b, rect)) {
    return true;
  }

  return false;
}

bool wire_collides_rect(const Component wire_component[static 1],
                        Rectangle rect) {
  Wire *wire = (Wire *)wire_component->ptr;
  if (wire->points_len < 2) {
    return false;
  }
  for (size_t i = 1; i < wire->points_len; i++) {
    if (line_collides_rect(wire->points[i - 1], wire->points[i], rect)) {
      return true;
    }
  }
  return false;
}
