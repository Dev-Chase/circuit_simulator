#include "component_io_pins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComponentIOPins component_io_pins_new(void) {
  return (ComponentIOPins){
#ifndef NDEBUG
      .pins = calloc(DEFAULT_COMPONENT_IO_PINS_LEN, sizeof(ComponentIOPin)),
#else
      .pins = malloc(DEFAULT_COMPONENT_IO_PINS_LEN * sizeof(ComponentIOPin)),
#endif
      .pin_len = 0,
      .pin_capacity = DEFAULT_COMPONENT_IO_PINS_LEN};
}

void component_io_pins_add(ComponentIOPins component_ios[static 1],
                           ComponentIOPin pin) {
  if (component_ios->pin_len == component_ios->pin_capacity) {
    component_ios->pin_capacity *= 2;
    component_ios->pins =
        realloc(component_ios->pins,
                sizeof(ComponentIOPin) * component_ios->pin_capacity);

    if (component_ios->pins == NULL) {
      perror("realloc failure");
      exit(EXIT_FAILURE);
    }
  }
  component_ios->pins[component_ios->pin_len] = pin;
  component_ios->pin_len++;
}

void component_io_pins_add_position(ComponentIOPins component_ios[static 1],
                                    Vector2 position) {
  component_io_pins_add(component_ios, (ComponentIOPin){.grid_pos = position});
}

void component_io_pins_add_positions(ComponentIOPins component_ios[static 1],
                                     size_t len,
                                     Vector2 positions[static len]) {
  for (size_t i = 0; i < len; i++) {
    component_io_pins_add_position(component_ios, positions[i]);
  }
}

bool cmp_pins(ComponentIOPin pin_a, ComponentIOPin pin_b) {
  return pin_a.amp == pin_b.amp && pin_a.res == pin_b.res &&
         pin_a.volt == pin_b.volt && pin_a.grid_pos.x == pin_b.grid_pos.x &&
         pin_a.grid_pos.y == pin_b.grid_pos.y;
}

void component_io_pins_remove(ComponentIOPins component_ios[static 1],
                              ComponentIOPin pin) {
  size_t ind;
  for (ind = 0; ind < component_ios->pin_len; ind++) {
    if (cmp_pins(component_ios->pins[ind], pin)) {
      break;
    }
  }

  if (ind == component_ios->pin_len) {
    fprintf(stderr, "index: %zu\n", ind);
    perror("Invalid Component Passed to component_group_remove");
    exit(EXIT_FAILURE);
  }

  if (ind != component_ios->pin_len) {
    memmove(&component_ios->pins[ind], &component_ios->pins[ind + 1],
            (component_ios->pin_len - ind - 1) * sizeof(size_t));
  }
  component_ios->pin_len--;
}

void component_io_pins_clear(ComponentIOPins component_ios[static 1]) {
  component_ios->pin_len = 0;
#ifndef NDEBUG
  memset(component_ios->pins, 0,
         component_ios->pin_capacity * sizeof(ComponentIOPin));
#endif
}

void component_io_pins_free(ComponentIOPins component_ios[static 1]) {
  free(component_ios->pins);
#ifndef NDEBUG
  component_ios->pins = NULL;
  component_ios->pin_len = 0;
  component_ios->pin_capacity = 0;
#endif
}
