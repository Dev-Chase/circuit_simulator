#include "action.h"
#include "button.h"
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
#define SIMULATION_AREA_X 0
#define SIMULATION_AREA_Y GRID_VAL_TO_COORD(3.2)
#define PLACE_LINE_WIDTH 3

// Functions
static void place_action_update(Simulation simulation[static 1],
                                Action place_action[static 1]) {

  if (action_activated(place_action) || *(bool *)place_action->data) {
  	*(bool *)place_action->data = true;
  	if (!memcmp(simulation->active_component, &(Component){0}, sizeof(Component))) {
			// create wire and place it
			// wire = wire_new(GRID_VEC_TO_COORD(GetMousePosition()), FG_COLOUR);
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
    .txt = NULL,
    .RECT = (Rectangle){SIMULATION_AREA_X, SIMULATION_AREA_Y, WIDTH,
                        HEIGHT - SIMULATION_AREA_Y},
};

static Action PLACE_ACTION = {
    .data = NULL,
    .button = &PLACE_BUTTON,
    .shortcut_cond = NULL,
    .UPDATE_FN = place_action_update,
    .RENDER_FN = place_action_render,
};

Action *place_action_init(void) {
  action_init(&PLACE_ACTION, sizeof(bool));
  assert(sizeof(bool) == 1);
  memset(PLACE_ACTION.data, false, sizeof(bool));

  return &PLACE_ACTION;
}
