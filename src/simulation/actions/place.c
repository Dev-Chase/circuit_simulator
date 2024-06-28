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
#define PLACE_X 9
#define PLACE_Y 1

typedef enum PlaceMode { NotPlacing, Creating, Positioning } PlaceMode;

// Functions
static bool place_action_shortcut(void) {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), SIMULATION_AREA_RECT);
}

void place_active_component(Simulation simulation[static 1],
                            Action place_action[static 1]) {
  if (*(PlaceMode *)place_action->data == Creating) {
    Wire *wire = (Wire *)simulation->active_component.ptr;
    wire_del_point(&simulation->active_component, wire->points_len - 1);
  }

  circuit_add_component(&simulation->circuit, &simulation->active_component);
  simulation->active_component = (Component){0};
  *(PlaceMode *)place_action->data = NotPlacing;
  puts("Finished");
}

static void add_placement(Simulation simulation[static 1],
                          Action place_action[static 1]) {
  Vector2 point = CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS;
  printf("Point: {%f, %f}\n", point.x, point.y);
  Wire *wire = (Wire *)simulation->active_component.ptr;
  if (wire_contains_point(wire, point, true)) {
    place_active_component(simulation, place_action);
    return;
  }
  wire_add_point(&simulation->active_component, &point);
  wire_add_point(&simulation->active_component, &point);
}

static void start_placement(Simulation simulation[static 1],
                            Action place_action[static 1]) {
  // start creating
  if (!memcmp(&simulation->active_component, &(Component){0},
              sizeof(Component)) &&
      CheckCollisionPointRec(GetMousePosition(), SIMULATION_AREA_RECT)) {
    *(PlaceMode *)place_action->data = Creating;
    puts("Making new wire");
    simulation->active_component = wire_new(WIRE_COLOUR);
    add_placement(simulation, place_action);
  } else {
    *(PlaceMode *)place_action->data = Positioning;
    puts("Placing Active Component");
    // place active component at Grid pos that corresponds to mouse pos
  }
}

void cancel_active_component(Simulation simulation[static 1],
                             Action place_action[static 1]) {
  COMPONENT_FN(simulation->active_component, free);
  simulation->active_component = (Component){0};
  *(PlaceMode *)place_action->data = NotPlacing;
  puts("Cancelled");
}

static void place_action_update(Simulation simulation[static 1],
                                Action place_action[static 1]) {
  if (action_activated(place_action)) {
    switch (*(PlaceMode *)place_action->data) {
    case NotPlacing:
      start_placement(simulation, place_action);
      break;
    case Creating:
      add_placement(simulation, place_action);
      break;
    case Positioning:
      // TODO: place active_component
      place_active_component(simulation, place_action);
      break;
    }
  }

  if (*(PlaceMode *)place_action->data != NotPlacing) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      cancel_active_component(simulation, place_action);
    }
    if (*(PlaceMode *)place_action->data == Creating) {
      Vector2 dest_pos =
          Vector2Clamp(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                       VEC2(0, SIMULATION_AREA_Y), VEC2(WIDTH, HEIGHT));
      wire_set_last((Wire *)simulation->active_component.ptr, dest_pos);

      if (IsKeyPressed(KEY_ENTER)) {
        place_active_component(simulation, place_action);
      }
    }
  }
}

static void place_action_render(const Action *const _) {
  // Draw
  DrawLineEx(VEC2(SIMULATION_AREA_X, SIMULATION_AREA_Y - 3),
             VEC2(SIMULATION_AREA_X + WIDTH, SIMULATION_AREA_Y - 3),
             PLACE_LINE_WIDTH, FG_COLOUR);
}

// Button
static Button PLACE_BUTTON = {
    .txt = PLACE_TXT,
    .RECT = RECT(GRID_VAL_TO_COORD(PLACE_X), GRID_VAL_TO_COORD(PLACE_Y),
                 BTN_WIDTH, BTN_HEIGHT),
    .fg = FG_COLOUR,
    .bg = BG_COLOUR,
};

static Action PLACE_ACTION = {
    .data = NULL,
    .button = &PLACE_BUTTON,
    .shortcut_cond = place_action_shortcut,
    .UPDATE_FN = place_action_update,
    .RENDER_FN = place_action_render,
};

Action *place_action_init(void) {
  action_init(&PLACE_ACTION, sizeof(PlaceMode));
  *(PlaceMode *)PLACE_ACTION.data = NotPlacing;

  return &PLACE_ACTION;
}
