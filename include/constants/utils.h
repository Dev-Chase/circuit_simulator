#ifndef UTILS_H
#define UTILS_H

#include "constants.h"
#include <math.h>

#ifdef _cplusplus
extern "C" {
#endif

// Utility Functions
Vector2 vector2_min(Vector2 a, Vector2 b);

// Utility Macros
#define VECTOR2(x_val, y_val)                                                  \
  (Vector2) { .x = x_val, .y = y_val }
#define VEC2 VECTOR2
#define RECT(x, y, w, h)                                                       \
  (Rectangle) { x, y, w, h }

#define GRID_VAL_TO_COORD(val) val *BLOCK_SIZE
#define GRID_TO_COORD(i, j) VECTOR2(i *BLOCK_SIZE, j *BLOCK_SIZE)
#define GRID_VEC_TO_COORD(vec) VECTOR2(vec.x *BLOCK_SIZE, vec.y *BLOCK_SIZE)

#define COORD_VAL_TO_GRID(val) (float)((int)round(val / BLOCK_SIZE))
#define COORD_TO_GRID(x, y) VECTOR2(COORD_VAL_TO_GRID(x), COORD_VAL_TO_GRID(y))
#define COORD_VEC_TO_GRID(vec)                                                 \
  VECTOR2(COORD_VAL_TO_GRID(vec.x), COORD_VAL_TO_GRID(vec.y))

#define CLOSEST_VALID_GRID_VAL_FROM(x) GRID_VAL_TO_COORD(COORD_VAL_TO_GRID(x))

#define CLOSEST_VALID_GRID_VEC_FROM_MOUSE_POS                                  \
  VEC2(CLOSEST_VALID_GRID_VAL_FROM(GetMouseX()),                               \
       CLOSEST_VALID_GRID_VAL_FROM(GetMouseY()))

#ifdef _cplusplus
}
#endif
#endif
