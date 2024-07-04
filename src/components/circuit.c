#include "circuit.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CIRCUIT_COMPONENT_LEN 20
Component circuit_new(const char *path, float tile_width, float tile_height) {
  Component component = (Component){
      .ptr = malloc(sizeof(Circuit)),

      // Data Manipulation
      .add_data = circuit_add_component,
      .del_data = circuit_del_component,
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

void circuit_add_component(const Component circuit_component[static 1],
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

void circuit_del_component(const Component circuit_component[static 1],
                           size_t ind) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;

  assert(circuit->components[ind].ptr != NULL);
  COMPONENT_FN(circuit->components[ind], free)

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

void circuit_del_components(const Component circuit_component[static 1],
                            ComponentGroup components[static 1]) {
  qsort(components->component_is, components->component_len, sizeof(size_t),
        compare_indeces);
  for (size_t i = 0; i < components->component_len; i++) {
    circuit_del_component(circuit_component, components->component_is[i]);
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
  // TODO: implement abstracted drawing

  Circuit *circuit = (Circuit *)circuit_component->ptr;
  // for (size_t i = 0; i < circuit->components_len; i++) {
  //   // printf("Rendered %zu:\n", i);
  //   // TODO: fix first element not rendering
  //   COMPONENT_FN(circuit->components[i], render);
  // }
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
