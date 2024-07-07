#include "action.h"
#include "circuit.h"
#include "component.h"
#include "component_group.h"
#include "raylib.h"
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

static void select_hovered(Simulation simulation[static 1]) {
  if (simulation->hovered.len > 0) {
    // component_group_clear(&simulation->selected);
    for (size_t i = 0; i < simulation->hovered.len; i++) {
      if (component_group_contains(&simulation->removing,
                                   simulation->hovered.ids[i])) {
        component_group_remove(&simulation->selected,
                               simulation->hovered.ids[i]);
      } else {
        component_group_add(&simulation->selected, simulation->hovered.ids[i]);
      }
    }
  }
}

static void select_action_cancel(Simulation _[static 1],
                                 Action select_action[static 1]) {
  select_action->active = false;
}

static void select_action_active_update(Simulation simulation[static 1],
                                        Action select_action[static 1]) {
  Vector2 start_mouse_pos = *(Vector2 *)select_action->data;
  Vector2 mouse_pos = vector2_constrain_to_simulation_area(GetMousePosition());
  Vector2 top_left = vector2_min(start_mouse_pos, mouse_pos);

  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;
  for (size_t i = 0; i < sim_circuit->components_len; i++) {
    if (COMPONENT_FN_RETURNS_PARAMS(
            sim_circuit->components[i], collides_rect,
            RECT(top_left.x, top_left.y, fabsf(mouse_pos.x - start_mouse_pos.x),
                 fabsf(mouse_pos.y - start_mouse_pos.y)))) {
      component_group_add(&simulation->hovered, sim_circuit->components[i].id);
      if (component_group_contains(&simulation->selected,
                                   sim_circuit->components[i].id)) {
        component_group_add(&simulation->removing,
                            sim_circuit->components[i].id);
      }
    }
  }
}

static void select_action_inactive_update(Simulation simulation[static 1]) {
  Circuit *sim_circuit = (Circuit *)simulation->circuit.ptr;
  if (simulation->hovered_i != SIZE_T_MAX) {
    component_group_add(&simulation->hovered,
                        sim_circuit->components[simulation->hovered_i].id);
    if (component_group_contains(
            &simulation->selected,
            sim_circuit->components[simulation->hovered_i].id) &&
        (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) {
      component_group_add(&simulation->removing,
                          sim_circuit->components[simulation->hovered_i].id);
    }
  }

  if (simulation->hovered_i != SIZE_T_MAX &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
      CheckCollisionPointRec(CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS,
                             SIMULATION_AREA_RECT)) {
    if (component_group_contains(&simulation->removing,
                                 sim_circuit->components[simulation->hovered_i].id)) {
      component_group_remove(&simulation->selected, sim_circuit->components[simulation->hovered_i].id);
    } else {
      component_group_add(&simulation->selected, sim_circuit->components[simulation->hovered_i].id);
    }
  }
}

static void select_action_update(Simulation simulation[static 1],
                                 Action select_action[static 1]) {
  if (action_activated(simulation, select_action) &&
      simulation->hovered_i == SIZE_T_MAX) {
    select_action->active = true;
    *(Vector2 *)select_action->data = GetMousePosition();
  }

  component_group_clear(&simulation->hovered);
  component_group_clear(&simulation->removing);
  if (select_action->active) {
    if (simulation_should_highlight(simulation)) {
      select_action_active_update(simulation, select_action);
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      select_action_cancel(simulation, select_action);
      return;
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      select_hovered(simulation);
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
    Vector2 mouse_pos =
        vector2_constrain_to_simulation_area(GetMousePosition());
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
