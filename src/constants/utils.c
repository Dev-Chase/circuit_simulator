#include "utils.h"
#include "raymath.h"
#include "simulation.h"

Vector2 vector2_min(Vector2 a, Vector2 b) {
  return VEC2(fminf(a.x, b.x), fminf(a.y, b.y));
}

Vector2 vector2_constrain_to_simulation_area(Vector2 point) {
  return Vector2Clamp(point, VEC2(0, SIMULATION_AREA_Y), VEC2(WIDTH, HEIGHT));
}
