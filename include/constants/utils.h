#ifndef UTILS_H
#define UTILS_H

#include "constants.h"
#include <math.h>

#ifdef _cplusplus
extern "C" {
#endif

// Utility Macros
#define VECTOR2(x_val, y_val)                                                  \
  (Vector2) { .x = x_val, .y = y_val }
#define VEC2 VECTOR2
#define GRID_VAL_TO_COORD(val) val *BLOCK_SIZE
#define GRID_TO_COORD(i, j) VECTOR2(i *BLOCK_SIZE, j *BLOCK_SIZE)
#define GRID_VEC_TO_COORD(vec) VECTOR2(vec.x *BLOCK_SIZE, vec.y *BLOCK_SIZE)

#define COORD_VAL_TO_GRID(val) (float)((int)round(val / BLOCK_SIZE))
#define COORD_TO_GRID(x, y) VECTOR2(COORD_VAL_TO_GRID(x), COORD_VAL_TO_GRID(y))
#define COORD_VEC_TO_GRID(vec)                                                 \
  VECTOR2(COORD_VAL_TO_GRID(vec.x), COORD_VAL_TO_GRID(vec.y))

#define CLOSEST_GRID_FROM_MOUSE_POS                                            \
  VEC2(GRID_VAL_TO_COORD(COORD_VAL_TO_GRID(GetMouseX())),                      \
       GRID_VAL_TO_COORD(COORD_VAL_TO_GRID(GetMouseY())))

#ifdef _cplusplus
}
#endif
#endif
