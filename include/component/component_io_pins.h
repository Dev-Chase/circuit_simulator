#ifndef COMPONENT_ENDS_H
#define COMPONENT_ENDS_H

#include "raylib.h"
#include <stddef.h>
#ifdef _cplusplus
extern "C" {
#endif
#define DEFAULT_COMPONENT_IO_PINS_LEN 20

typedef struct ComponentIOPin {
  Vector2 grid_pos;
  float volt;
  float amp;
  float res;
} ComponentIOPin;

typedef struct ComponentIOPins {
  ComponentIOPin *pins;
  size_t pin_len;
  size_t pin_capacity;
} ComponentIOPins;

ComponentIOPins component_io_pins_new(void);
void component_io_pins_add(ComponentIOPins component_ios[static 1],
                           ComponentIOPin pin);
void component_io_pins_add_position(ComponentIOPins component_ios[static 1],
                                    Vector2 position);
void component_io_pins_add_positions(ComponentIOPins component_ios[static 1],
                                     size_t len, Vector2 positions[static len]);
void component_io_pins_remove(ComponentIOPins component_ios[static 1],
                              ComponentIOPin pin);
void component_io_pins_clear(ComponentIOPins component_ios[static 1]);
void component_io_pins_free(ComponentIOPins component_ios[static 1]);

#ifdef _cplusplus
}
#endif
#endif
