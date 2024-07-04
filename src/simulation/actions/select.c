#include "action.h"
#include "circuit.h"
#include "component.h"
#include "component_group.h"
#include "raylib.h"
#include "raymath.h"
#include "simulation.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

// Functions
static bool select_action_shortcut(void) {
  return (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
         IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
         CheckCollisionPointRec(GetMousePosition(), SIMULATION_AREA_RECT);
}

static void select_action_cancel(Simulation simulation[static 1],
                                 Action select_action[static 1]) {
  if (simulation->hovered.component_len > 0) {
    component_group_clear(&simulation->selected);
    for (size_t i = 0; i < simulation->hovered.component_len; i++) {
      component_group_add(&simulation->selected,
                          simulation->hovered.component_is[i]);
    }
  }
  select_action->active = false;
}

static void select_action_active_update(Simulation simulation[static 1],
                                        Action select_action[static 1]) {
  Vector2 start_mouse_pos = *(Vector2 *)select_action->data;
  Vector2 mouse_pos = Vector2Clamp(
      GetMousePosition(), VEC2(0, SIMULATION_AREA_Y), VEC2(WIDTH, HEIGHT));
  Vector2 top_left = vector2_min(start_mouse_pos, mouse_pos);

  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;
  for (size_t i = 0; i < sim_circuit->components_len; i++) {
    if (COMPONENT_FN_RETURNS_PARAMS(
            sim_circuit->components[i], collides_rect,
            RECT(top_left.x, top_left.y, fabsf(mouse_pos.x - start_mouse_pos.x),
                 fabsf(mouse_pos.y - start_mouse_pos.y)))) {
      component_group_add(&simulation->hovered, i);
    }
  }
}

static void select_action_inactive_update(Simulation simulation[static 1]) {
  if (simulation->hovered_i != SIZE_T_MAX) {
    component_group_add(&simulation->hovered, simulation->hovered_i);
  }

  if (simulation->hovered_i != SIZE_T_MAX &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                             SIMULATION_AREA_RECT)) {
    if (!component_group_contains(&simulation->selected,
                                  simulation->hovered_i)) {
      component_group_add(&simulation->selected, simulation->hovered_i);
    } else {
      component_group_remove(&simulation->selected, simulation->hovered_i);
    }
  }
}

static void select_action_update(Simulation simulation[static 1],
                                 Action select_action[static 1]) {
  if (action_activated(simulation, select_action) &&
      simulation->hovered_i == SIZE_T_MAX) {
    action_activate(simulation, select_action);
    *(Vector2 *)select_action->data = CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS;
  }

  component_group_clear(&simulation->hovered);
  if (select_action->active) {
    if (simulation_should_highlight(simulation)) {
      select_action_active_update(simulation, select_action);
    }

    if (IsKeyPressed(KEY_ESCAPE) || !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      select_action_cancel(simulation, select_action);
      return;
    }
  } else {
    if (simulation_should_highlight(simulation)) {
      select_action_inactive_update(simulation);
    }
  }
}

static void select_action_render(const Simulation _[static 1],
                                 const Action select_action[static 1]) {
  if (select_action->active) {
    Vector2 start_mouse_pos = *(Vector2 *)select_action->data;
    Vector2 mouse_pos = Vector2Clamp(
        GetMousePosition(), VEC2(0, SIMULATION_AREA_Y), VEC2(WIDTH, HEIGHT));
    Vector2 top_left = vector2_min(start_mouse_pos, mouse_pos);
    DrawRectangle(
        top_left.x, top_left.y, fabsf(mouse_pos.x - start_mouse_pos.x),
        fabsf(mouse_pos.y - start_mouse_pos.y), (Color){255, 255, 255, 100});
    DrawRectangleLines(top_left.x, top_left.y,
                       fabsf(mouse_pos.x - start_mouse_pos.x),
                       fabsf(mouse_pos.y - start_mouse_pos.y), WHITE);
  }
}

static Button SELECT_BUTTON = {
    .txt = NULL,
};

static Action SELECT_ACTION = {
    .data = NULL,
    .active = false,
    .allow_selection = true,
    .prevent_directly_switching_action = false,
    .button = &SELECT_BUTTON,
    .shortcut_cond = select_action_shortcut,
    .CANCEL_FN = select_action_cancel,
    .UPDATE_FN = select_action_update,
    .RENDER_FN = select_action_render,
};

Action *select_action_init(void) {
  // Initializing
  action_init(&SELECT_ACTION, sizeof(Vector2));
  // *(size_t *)SELECT_ACTION.data = SIZE_T_MAX;

  return &SELECT_ACTION;
}
