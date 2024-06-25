#include "circuit.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: implement save, run, render_run, & render
Component circuit_component(Circuit ptr[static 1]) {
  return (Component){
      .ptr = (void *)ptr,

      .add_data = circuit_add_component,
      .del_data = circuit_del_component,
      .clear = circuit_clear,
      .free = circuit_free,

      // .save = circuit_save,
      // .run = circuit_run,
      .save = NULL,
      .run = NULL,
      .place = circuit_place,

      .render = circuit_render,
      // .render_run = circuit_render_run,
      .render_run = NULL,
  };
}

void circuit_fill_new(Circuit circuit[static 1], const char *path) {
#define DEFAULT_CIRCUIT_COMPONENT_LEN 20
#define DEFAULT_CIRCUIT_COMPONENT_DATA_SIZE 100
  circuit->path = path,

  circuit->components =
      malloc(sizeof(Component) * DEFAULT_CIRCUIT_COMPONENT_LEN),
  circuit->component_data = NULL;

  circuit->components_len = 0;

  circuit->components_capacity = DEFAULT_CIRCUIT_COMPONENT_LEN;
  circuit->component_data_size = 0;

  circuit->this_component = circuit_component(circuit);
}

void circuit_add_component(const Component circuit_component[static 1],
                           const void *const COMPONENT, size_t size) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  if (circuit->components_len == circuit->components_capacity) {
    circuit->components_capacity *= 2;
    circuit->components =
        realloc(circuit->components, circuit->components_capacity);

    if (circuit->components == NULL || circuit->component_data) {
      perror("realloc fail!");
      exit(EXIT_FAILURE);
    }
  }
  circuit->component_data =
      realloc(circuit->component_data,
              circuit->component_data_size + size + sizeof(size_t));
  *(size_t *)&circuit->component_data[circuit->component_data_size] = 0;

  memcpy(&circuit->component_data[circuit->component_data_size] +
             sizeof(size_t),
         ((Component *)COMPONENT)->ptr, size);
  circuit->component_data_size += (size + sizeof(size_t));

  circuit->components[circuit->components_len] = *(Component *)COMPONENT;
  circuit->components_len++;
}

void circuit_del_component(const Component circuit_component[static 1],
                           size_t ind) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;

#ifndef NDEBUG
  memset(&circuit->components[ind], 0, sizeof(circuit->components[ind]));
#endif
  if (circuit->components_len != ind + 1) {
    memmove(&circuit->components[ind], &circuit->components[ind + 1],
            (circuit->components_len - ind - 1) *
                sizeof(circuit->components[ind]));
  }

  size_t place = 0;
  for (size_t i = 0; i < ind; i++) {
    place += *(size_t *)&circuit->component_data[place];
  }
  size_t chunk_size = *(size_t *)&circuit->component_data[place];
  memmove(&circuit->component_data[place],
          &circuit->component_data[place + chunk_size] + sizeof(size_t),
          chunk_size + sizeof(size_t));

  circuit->component_data =
      realloc(circuit->component_data,
              circuit->component_data_size - chunk_size - sizeof(size_t));
  if (circuit->component_data == NULL) {
    perror("realloc fail!");
    exit(EXIT_FAILURE);
  }

  circuit->components_len--;
}

void circuit_clear(const Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  circuit->components_len = 0;

  free(circuit->component_data);
  circuit->component_data = NULL;
  circuit->component_data_size = 0;

#ifndef NDEBUG
  memset(circuit->components, 0,
         circuit->components_len * sizeof(circuit->components[0]));
#endif
}

void circuit_free(const Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  free(circuit->components);
  free(circuit->component_data);

#ifndef NDEBUG
  circuit->component_data_size = 0;
  circuit->component_data = NULL;
  circuit->components = NULL;
  circuit->components_len = 0;
  circuit->components_capacity = 0;
#endif
}

// save, run

void circuit_place(Component circuit_component[static 1], Vector2 grid_pos) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  circuit->pos = GRID_VEC_TO_COORD(grid_pos);
}

void circuit_render(const Component circuit_component[static 1]) {
  Circuit *circuit = (Circuit *)circuit_component->ptr;
  for (size_t i = 0; i < circuit->components_len; i++) {
    COMPONENT_FN(circuit->components[i], render);
  }
}

// render_run
