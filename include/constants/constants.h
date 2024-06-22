#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Window
#define FPS 60
#define BLOCK_SIZE 25
#define N_COLS 36
#define N_ROWS 30
#define WIDTH N_COLS *BLOCK_SIZE
#define HEIGHT N_ROWS *BLOCK_SIZE
#define WIN_NAME "Equation Visualizer"

// Grid & Drawing
#define FONT_SIZE 20
#define FG_COLOUR WHITE
#define BG_COLOUR BLACK
#define PALE_GRAY                                                              \
	(Color) { 33, 27, 33, 150 }
#define GRID_COLOUR PALE_GREY
#define GRID_LINE_WIDTH 1.0F
#define GRID_GAP 25

#ifdef __cplusplus
}
#endif
#endif // !CONSTANTS_H
