#ifndef UTILS_H
#define UTILS_H

#include "constants.h"

#ifdef _cplusplus
extern "C" {
#endif

// Utility Macros
#define VECTOR2(x_val, y_val)                                                  \
	(Vector2) { .x = x_val, .y = y_val }
#define VEC2 VECTOR2
#define GRID_VAL_TO_COORD(val) val *BLOCK_SIZE
#define GRID_TO_COORD(i, j) VECTOR2(i *BLOCK_SIZE, j *BLOCK_SIZE)
#define COORD_VAL_TO_GRID(val) (int)round(val / BLOCK_SIZE)
#define COORD_TO_GRID(x, y)                                                    \
	VECTOR2((int)round(x / BLOCK_SIZE), (int)round(y / BLOCK_SIZE))

#ifdef _cplusplus
}
#endif
#endif
