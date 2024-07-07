#include "circuit.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#define DEFAULT_CIRCUIT_COMPONENT_LEN 20
Component circuit_new(const char *path, float tile_width, float tile_height) {
  Component component = (Component){
      .ptr = malloc(sizeof(Circuit)),

      // Data Manipulation
      .add_data = circuit_append_component,
      .del_data = circuit_remove_component,
      .clear = circuit_clear,
      .free = circuit_free,

      // Actions
      // .save = circuit_save,
      // .run = circuit_run,
      .place = circuit_place,

      // Rendering
      .render = circuit_render,
      // .render_run = circuit_render_run,
      .render_run = NULL,

      // Information
      .is_hovered = circuit_is_hovered,
      .collides_rect = circuit_collides_rect,
  };
  uuid_generate_random(component.id);

  *(Circuit *)component.ptr = (Circuit){
      .path = path,
      .components = malloc(sizeof(Component) * DEFAULT_CIRCUIT_COMPONENT_LEN),
      .components_len = 0,
      .components_capacity = DEFAULT_CIRCUIT_COMPONENT_LEN,
      .rect = RECT(0, 0, GRID_VAL_TO_COORD(tile_width),
                   GRID_VAL_TO_COORD(tile_height)),
  };

  return component;
}

void circuit_append_component(const Component circuit_component[static 1],
                              const void *const COMPONENT) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  if (circuit->components_len > circuit->components_capacity) {
    circuit->components_capacity *= 2;
    circuit->components = realloc(
        circuit->components, circuit->components_capacity * sizeof(Component));
    if (circuit->components == NULL) {
      perror("malloc fail!");
      exit(EXIT_FAILURE);
    }
  }

  circuit->components[circuit->components_len] = *(Component *)COMPONENT;
  circuit->components_len++;
}

size_t circuit_get_i_from_id(const Circuit circuit[static 1], uuid_t id) {
  size_t i;
  for (i = 0; i < circuit->components_len; i++) {
    if (uuid_compare(circuit->components[i].id, id) == 0) {
      break;
    }
  }

  if (i == circuit->components_len) {
    perror("invalid ID");
    exit(EXIT_FAILURE);
  }

  return i;
}

// NOTE: does not free
void circuit_remove_component(const Component circuit_component[static 1],
                              size_t ind) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;

  assert(circuit->components[ind].ptr != NULL);
  // COMPONENT_FN(circuit->components[ind], free)

#ifndef NDEBUG
  memset(&circuit->components[ind], 0, sizeof(Component));
#endif
  if (circuit->components_len != ind + 1) {
    memmove(&circuit->components[ind], &circuit->components[ind + 1],
            (circuit->components_len - ind - 1) * sizeof(Component));
  }

  circuit->components_len--;
}

int compare_indeces(const void *a, const void *b) {
  return *(size_t *)a < *(size_t *)b;
}

// NOTE: does not free
void circuit_remove_group(const Component circuit_component[static 1],
                          ComponentGroup group[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;

  size_t *indexes = (size_t *)malloc(sizeof(size_t) * group->len);
  puts("------------------------------------------");
  puts("Deleting the Following:");
  for (size_t i = 0; i < group->len; i++) {
    indexes[i] = circuit_get_i_from_id(circuit, group->ids[i]);
    printf("%p\n", &circuit->components[indexes[i]]);
  }
  puts("------------------------------------------");
  qsort(indexes, group->len, sizeof(size_t), compare_indeces);

  for (size_t i = 0; i < group->len; i++) {
    circuit_remove_component(circuit_component, indexes[i]);
  }
}

void circuit_clear(const Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  circuit->components_len = 0;

#ifndef NDEBUG
  memset(circuit->components, 0, circuit->components_len * sizeof(Component));
#endif
}

void circuit_free(Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  free(circuit->components);
  free(circuit_component->ptr);

#ifndef NDEBUG
  circuit_component->ptr = NULL;
#endif
}

// save, run

void circuit_place(Component circuit_component[static 1], Vector2 grid_pos) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  circuit->rect.x = GRID_VAL_TO_COORD(grid_pos.x);
  circuit->rect.y = GRID_VAL_TO_COORD(grid_pos.y);
}

void circuit_render(const Component circuit_component[static 1]) {
  // TODO: review

  Circuit *circuit = (Circuit *)circuit_component->ptr;
#define CIRCUIT_RECT_BORDER_WIDTH 2.5
  Rectangle smaller_rect = (Rectangle){
      circuit->rect.x + (CIRCUIT_RECT_BORDER_WIDTH / 2),
      circuit->rect.y + (CIRCUIT_RECT_BORDER_WIDTH / 2),
      circuit->rect.width - 2.5,
      circuit->rect.height - 2.5,
  };
  DrawRectangleRounded(circuit->rect, 5, 30, FG_COLOUR);
  DrawRectangleRounded(smaller_rect, 5, 30, BG_COLOUR);
}

// render_run

bool circuit_is_hovered(const Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  return CheckCollisionPointRec(GetMousePosition(), circuit->rect);
}

bool circuit_collides_rect(const Component circuit_component[static 1],
                           Rectangle rect) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;

  return CheckCollisionRecs(circuit->rect, rect);
}
