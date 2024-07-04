#include "action.h"
#include "button.h"
#include "circuit.h"
#include "component.h"
#include "constants.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include "wire.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default Action Data
#define PLACE_TXT "Place"
#define PLACE_LINE_WIDTH 3
#define PLACE_X GRID_VAL_TO_COORD(9)
#define PLACE_Y GRID_VAL_TO_COORD(1)

typedef enum PlaceMode { CreatingWire, Positioning } PlaceMode;

typedef struct PlaceInfo {
  PlaceMode mode;
  Component component;
} PlaceData;

// Event Functions
inline static bool check_valid_click(void) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), SIMULATION_AREA_RECT);
}

// Action Wide Functions
static void place_component(Simulation simulation[static 1],
                            Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  if (place_data->mode == Positioning) {
    COMPONENT_FN_PARAMS(place_data->component, place,
                        CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS)
  }
  circuit_add_component(&simulation->circuit, &place_data->component);

  place_action->prevent_directly_switching_action = false;
  place_data->mode = Positioning;
  place_data->component.ptr = NULL;
}

// Positioning
static void update_positioning(Simulation simulation[static 1],
                               Action place_action[static 1]) {
  if (IsKeyPressed(KEY_ENTER)) {
    place_component(simulation, place_action);
  }
}

// Creation
static void create_point(Action place_action[static 1], Vector2 point) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  wire_add_point(&place_data->component, &point);
  wire_add_point(&place_data->component, &point);
}

static void start_creation(Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  puts("------------------------");
  puts("Place Action - Creating!");
  place_action->prevent_directly_switching_action = true;
  place_data->mode = CreatingWire;

  place_data->component = wire_new(WIRE_COLOUR);
  create_point(place_action, CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS);
}

static void update_creation(Simulation simulation[static 1],
                            Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  Wire *wire = (Wire *)place_data->component.ptr;

  if (IsKeyPressed(KEY_ENTER)) {
    if (wire->points_len > 2) {
      wire_del_point(&place_data->component, wire->points_len - 1);
      place_component(simulation, place_action);
    }
    return;
  }

  if (check_valid_click()) {
    wire_del_point(&place_data->component, wire->points_len - 1);
    Vector2 mouse_pos = CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS;
    Vector2 last_point = wire->points[wire->points_len - 1];
    if (last_point.x == mouse_pos.x && last_point.y == mouse_pos.y) {
      place_component(simulation, place_action);
      return;
    }
    create_point(place_action, mouse_pos);
  }

  wire_set_last(wire, CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS);
}

//  Action Functions
inline static bool place_action_shortcut(void) { return IsKeyPressed(KEY_P); }

static void place_action_cancel(Simulation _[static 1],
                                Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  place_action->active = false;
  if (place_data->component.ptr != NULL) {
    COMPONENT_FN(place_data->component, free)
    place_data->component.ptr = NULL;
  }
}

static void place_action_update(Simulation simulation[static 1],
                                Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  if (!place_action->active) {
    if (action_activated(simulation, place_action)) {
      action_toggle(simulation, place_action);
      place_data->mode = Positioning;
    }
    return;
  }

  if (IsKeyPressed(KEY_ESCAPE) || action_activated(simulation, place_action)) {
    place_action_cancel(simulation, place_action);
    return;
  }

  if (place_data->mode == Positioning && place_data->component.ptr != NULL) {
    update_positioning(simulation, place_action);
  } else if (place_data->component.ptr == NULL) {
    if (check_valid_click()) {
      start_creation(place_action);
    }
  } else {
    update_creation(simulation, place_action);
  }
}

static void place_action_render(const Simulation _[static 1],
                                const Action place_action[static 1]) {
  // Draw
  DrawLineEx(VEC2(SIMULATION_AREA_X, SIMULATION_AREA_Y - 3),
             VEC2(SIMULATION_AREA_X + WIDTH, SIMULATION_AREA_Y - 3),
             PLACE_LINE_WIDTH, FG_COLOUR);

  if (place_action->active) {
    DrawCircleV(GetMousePosition(), 5, WIRE_COLOUR);

    PlaceData *place_data = (PlaceData *)place_action->data;
    if (place_data->component.ptr != NULL) {
      COMPONENT_FN(place_data->component, render);
    }
  }
}

// Button
static Button PLACE_BUTTON = {
    .txt = PLACE_TXT,
    .RECT = BTN_RECT(PLACE_X, PLACE_Y),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action PLACE_ACTION = {
    .data = NULL,
    .active = false,
    .allow_selection = false,
    .prevent_directly_switching_action = false,
    .button = &PLACE_BUTTON,
    .shortcut_cond = place_action_shortcut,
    .CANCEL_FN = place_action_cancel,
    .UPDATE_FN = place_action_update,
    .RENDER_FN = place_action_render,
};

Action *place_action_init(void) {
  action_init(&PLACE_ACTION, sizeof(PlaceData));
  ((PlaceData *)(PLACE_ACTION.data))->component.ptr = NULL;

  return &PLACE_ACTION;
}
