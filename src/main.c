#include "constants.h"
#include "raylib.h"
#include "simulation.h"
#include "utils.h"
#include <stdlib.h>

void draw_grid(void) {
	for (int i = 0; i < N_ROWS; i++) {
		DrawLineEx(VEC2(0.0F, (i + 1) * BLOCK_SIZE),
				   VEC2(WIDTH, (i + 1) * BLOCK_SIZE), GRID_LINE_WIDTH,
				   PALE_GRAY);
	}
	for (int i = 0; i < N_COLS; i++) {
		DrawLineEx(VEC2(i * BLOCK_SIZE, 0.0F), VEC2(i * BLOCK_SIZE, HEIGHT),
				   GRID_LINE_WIDTH, PALE_GRAY);
	}
}

int main(void) {
	// Initializing Window & Camera
	InitWindow(WIDTH, HEIGHT, WIN_NAME);
	SetTargetFPS(FPS);

	// Simulation State
	Simulation simulation = simulation_new();

	// Loop
	while (!WindowShouldClose()) {
		// Updating
		simulation_update(&simulation);

		// Drawing
		BeginDrawing();
		ClearBackground(BG_COLOUR);
		draw_grid();
		simulation_render(&simulation);

		EndDrawing();
	}
	CloseWindow();

	// Freeing Memory
	simulation_free(&simulation);

	return EXIT_SUCCESS;
}
