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
static bool place_action_shortcut(void) {
  return IsKeyPressed(KEY_P) &&
         (IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT));
}

static void place_action_update(Simulation simulation[static 1],
                                Action place_action[static 1]) {
  PlaceData *place_data = (PlaceData *)place_action->data;
  if (action_activated(simulation, place_action)) {
    puts("Place Action - Positioning!");
    place_action->active = true;
    place_data->mode = Positioning;
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                             SIMULATION_AREA_RECT) &&
      !simulation_other_action_active(simulation, place_action) &&
      !place_action->active) {
    puts("Place Action - Creating!");
    place_action->active = true;
    place_data->mode = CreatingWire;
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
    if (place_data->component.ptr != NULL) {
      puts("Rendering Placing Component");

      COMPONENT_FN(place_data->component, render);
    } else {
      // puts("shouldn't happen");
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
    .button = &PLACE_BUTTON,
    .shortcut_cond = place_action_shortcut,
    .UPDATE_FN = place_action_update,
    .RENDER_FN = place_action_render,
};

Action *place_action_init(void) {
  action_init(&PLACE_ACTION, sizeof(PlaceData));
  ((PlaceData *)(PLACE_ACTION.data))->component.ptr = NULL;

  return &PLACE_ACTION;
}
