#include "utils.h"

Vector2 vector2_min(Vector2 a, Vector2 b) {
  return VEC2(fminf(a.x, b.x), fminf(a.y, b.y));
}
