#include "action.h"
#include "button.h"
#include "circuit.h"
#include "component.h"
#include "constants.h"
#include "raylib.h"
#include "raymath.h"
#include "simulation.h"
#include "utils.h"
#include "wire.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default Action Data
#define SIMULATION_AREA_X 0
#define SIMULATION_AREA_Y GRID_VAL_TO_COORD(3)
#define SIMULATION_AREA_RECT                                                   \
  RECT(SIMULATION_AREA_X, SIMULATION_AREA_Y, WIDTH, HEIGHT - SIMULATION_AREA_Y)
#define PLACE_TXT "Place"
#define PLACE_LINE_WIDTH 3
#define PLACE_X GRID_VAL_TO_COORD(9)
#define PLACE_Y GRID_VAL_TO_COORD(1)

typedef enum PlaceMode { CreatingWire, Positioning } PlaceMode;

typedef struct PlaceInfo {
  PlaceMode mode;
  Component component;
} PlaceData;

// Functions
// static bool place_action_shortcut(void) {
//   return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
//          CheckCollisionPointRec(GetMousePosition(), SIMULATION_AREA_RECT);
// }

static bool place_action_shortcut(void) {
  return IsKeyPressed(KEY_P) &&
         (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT));
}

void place_component(Simulation simulation[static 1],
                     Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)(place_action->data);
  if (place_data->mode == CreatingWire) {
    Wire *wire = (Wire *)place_data->component.ptr;
    wire_del_point(&simulation->selected_component, wire->points_len - 1);
  }

  circuit_add_component(&simulation->circuit, &place_data->component);
  place_action->active = false;
  place_data->component = (Component){0};
  puts("Finished");
}

// NOTE: Here
static void create_point(Simulation simulation[static 1],
                         Action place_action[static 1], Vector2 point) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  printf("Point: {%f, %f}\n", point.x, point.y);

  Wire *wire = (Wire *)(place_data->component.ptr);
  Vector2 last_point = wire->points[wire_last_i(wire)];
  if (point.x == last_point.x && point.y == last_point.y) {
    place_component(simulation, place_action);
    return;
  }
  wire_add_point(&place_data->component, &point);
  wire_add_point(&place_data->component, &point);
}

static void start_creating_wire(Simulation simulation[static 1],
                                Action place_action[static 1]) {
  puts("Creating New Wire");
  // place_action->active = true;
  PlaceData *place_data = (PlaceData *)place_action->data;
  place_data->mode = CreatingWire;

  // Verify that placing component is zeroed
  // TODO: fix being tripped after second point entered
  // if (place_data->component.ptr != NULL) {
  //   perror("Placing Component was not NULl");
  //   exit(EXIT_FAILURE);
  // }

  place_data->component = wire_new(WIRE_COLOUR);
  Wire *wire = (Wire *)place_data->component.ptr;
  wire->is_last_set = true;
  create_point(simulation, place_action, CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS);
}

static void cancel_active_component(Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  COMPONENT_FN(place_data->component, free);

  place_data->component = (Component){0};
  place_action->active = false;
  puts("Cancelled Placing");
}

static void create_update(Simulation simulation[static 1],
                          Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;

  Wire *wire = (Wire *)place_data->component.ptr;
  Vector2 dest_pos =
      Vector2Clamp(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                   VEC2(0, SIMULATION_AREA_Y), VEC2(WIDTH, HEIGHT));
  wire_set_last(wire, dest_pos);

  Vector2 mouse_pos = CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS;
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(mouse_pos, SIMULATION_AREA_RECT)) {
    create_point(simulation, place_action, mouse_pos);
  }

  if (IsKeyPressed(KEY_ENTER)) {
    place_component(simulation, place_action);
  }
}

static void position_update(Simulation simulation[static 1],
                            Action place_action[static 1]) {
  if (IsKeyPressed(KEY_ENTER) ||
      (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
       CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                              SIMULATION_AREA_RECT))) {
    place_component(simulation, place_action);
  }
}

static void place_action_update(Simulation simulation[static 1],
                                Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  if (action_activated(simulation, place_action) && !place_action->active) {
    puts("Placing Active Component");
    // place_action->active = true;
    PlaceData *place_data = (PlaceData *)place_action->data;
    place_data->mode = Positioning;
    // place component at Grid pos that corresponds to mouse pos
  }

  // Create
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                             SIMULATION_AREA_RECT) &&
      !simulation_other_action_active(simulation, place_action) &&
      !place_action->active) {
    puts("Started to Create Wire !");
    printf("PlaceAction: %p\n", place_action);
    start_creating_wire(simulation, place_action);
  }

  if (place_action->active) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      cancel_active_component(place_action);
    }

    switch (place_data->mode) {
    case CreatingWire:
      create_update(simulation, place_action);
      break;
    case Positioning:
      position_update(simulation, place_action);
      break;
    }
  }
}

static void place_action_render(const Simulation _[static 1],
                                const Action place_action[static 1]) {
  // Draw
  DrawLineEx(VEC2(SIMULATION_AREA_X, SIMULATION_AREA_Y - 3),
             VEC2(SIMULATION_AREA_X + WIDTH, SIMULATION_AREA_Y - 3),
             PLACE_LINE_WIDTH, FG_COLOUR);

  PlaceData *place_data = (PlaceData *)place_action->data;
  if (place_action->active) {
    COMPONENT_FN(place_data->component, render);
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
    .button = &PLACE_BUTTON,
    .shortcut_cond = place_action_shortcut,
    .UPDATE_FN = place_action_update,
    .RENDER_FN = place_action_render,
};

Action *place_action_init(void) {
  action_init(&PLACE_ACTION, sizeof(PlaceData));
  // ((PlaceData *)(PLACE_ACTION.data))->component = (Component){0};

  return &PLACE_ACTION;
}
