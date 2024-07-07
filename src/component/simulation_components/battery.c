#include "battery.h"
#include "component_io_pins.h"
#include "raylib.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <uuid/uuid.h>

Component battery_new(int voltage, Rectangle rect) {
  Component component = (Component){
      .ptr = malloc(sizeof(Battery)),

      // Data Manipulation
      .add_data = BATTERY_ADD,
      .del_data = BATTERY_DEL,
      .clear = BATTERY_CLEAR,
      .free = BATTERY_FREE,

      // Actions
      // .save = battery_save,
      // .run = battery_run,
      .place = battery_place,

      // Rendering
      .render = battery_render,
      // .render_run = battery_render_run,

      // Information
      .is_hovered = battery_is_hovered,
      .collides_rect = battery_collides_rect,

      .id = {0},
  };
  uuid_generate_random(component.id);

  *(Battery *)component.ptr = (Battery){
      .voltage = voltage,
      .rect = rect,
  };

  return component;
}

// Action Functions
// save

void battery_run(Component battery_component[static 1]) {
  Battery *battery = (Battery *)battery_component->ptr;
  assert(battery_component->pins.pin_len == 2);
  battery_component->pins.pins[0].volt = battery->voltage;
  battery_component->pins.pins[1].volt = -battery->voltage;
}

void battery_place(Component battery_component[static 1], Vector2 grid_pos) {
  Battery *battery = (Battery *)battery_component->ptr;

  battery->rect.x = GRID_VAL_TO_COORD(grid_pos.x);
  battery->rect.y = GRID_VAL_TO_COORD(grid_pos.y);
  Vector2 first_pin =
      battery->is_horizontal
          ? VEC2(battery->rect.x, battery->rect.y + (battery->rect.height / 2))
          : VEC2(battery->rect.x + (battery->rect.width / 2), battery->rect.y);
  Vector2 second_pin = battery->is_horizontal
                           ? VEC2(battery->rect.x + battery->rect.width,
                                  battery->rect.y + (battery->rect.height / 2))
                           : VEC2(battery->rect.x + (battery->rect.width / 2),
                                  battery->rect.y + battery->rect.height);

  component_io_pins_add_position(&battery_component->pins, first_pin);
  component_io_pins_add_position(&battery_component->pins, second_pin);
}

// Rendering
void battery_render(const Component battery_component[static 1]) {
  Battery *battery = (Battery *)battery_component->ptr;
  assert(battery_component->pins.pin_len == 2);
  Vector2 first = battery_component->pins.pins[0].grid_pos;
  Vector2 second = battery_component->pins.pins[1].grid_pos;

  DrawLineEx(first, second, BATTERY_LINE_WIDTH, BATTERY_COLOUR);

  Vector2 big_line_a;
  Vector2 big_line_b;
  Vector2 small_line_a;
  Vector2 small_line_b;
  if (battery->is_horizontal) {
    float middle = battery->rect.x + (battery->rect.width / 2);
    big_line_a = VEC2(middle - BATTERY_LINE_WIDTH - 2, battery->rect.y);
    big_line_b = VEC2(middle - BATTERY_LINE_WIDTH - 2,
                      battery->rect.y + battery->rect.height);
    small_line_a = VEC2(middle + BATTERY_LINE_WIDTH + 2,
                        battery->rect.y + BATTERY_LINE_WIDTH);
    small_line_b =
        VEC2(middle + BATTERY_LINE_WIDTH + 2,
             battery->rect.y + battery->rect.height - BATTERY_LINE_WIDTH);
  } else {
    float middle = battery->rect.y + (battery->rect.height / 2);
    big_line_a = VEC2(battery->rect.x, middle - BATTERY_LINE_WIDTH - 2);
    big_line_b = VEC2(battery->rect.x + battery->rect.width,
                      middle - BATTERY_LINE_WIDTH - 2);
    small_line_a = VEC2(battery->rect.x, middle + BATTERY_LINE_WIDTH + 2);
    small_line_b = VEC2(battery->rect.x + battery->rect.width,
                        middle + BATTERY_LINE_WIDTH + 2);
  }

  DrawLineEx(big_line_a, big_line_b, BATTERY_LINE_WIDTH, BATTERY_COLOUR);
  DrawLineEx(small_line_a, small_line_b, BATTERY_LINE_WIDTH, BATTERY_COLOUR);
}

// .render_run = battery_render_run,

// Information
bool battery_is_hovered(const Component battery_component[static 1]) {
  Battery *battery = (Battery *)battery_component->ptr;
  return CheckCollisionPointRec(GetMousePosition(), battery->rect);
}

bool battery_collides_rect(const Component battery_component[static 1],
                           Rectangle rect) {
  Battery *battery = (Battery *)battery_component->ptr;
  return CheckCollisionRecs(battery->rect, rect);
}
