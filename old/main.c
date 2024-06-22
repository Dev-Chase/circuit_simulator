#include "constants.h"
#include "raylib.h"
#include "wire_old.h"
#include <stdio.h>
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

// Simulation Variables
#define WIRE_LEN 10
	// TODO: replace with Circuit struct & list utility
	Wire *wires = malloc(sizeof(Wire) * WIRE_LEN);
	size_t wires_len = 0;
	wires[0] = wire_new(VEC2(WIDTH / 2, HEIGHT / 2), false, 0, NULL);
	wires_len++;
	wire_add_point(&wires[0], VEC2((WIDTH / 2) + 100, HEIGHT / 2));
	wire_add_point(&wires[0], VEC2((WIDTH / 2) + 100, (HEIGHT / 2) + 200));

	// Loop
	while (!WindowShouldClose()) {
		// Updating
		if (IsKeyPressed(KEY_A)) {
			wires_len++;
			printf("Index Accessed: %zu\n", wires_len - 1);
			printf("Wires Length Exceeds Len?: %hhu\nWIRE_LEN: %d\n",
				   (wires_len - 1) > WIRE_LEN, WIRE_LEN);
			wires[wires_len - 1] =
				wire_new(VEC2(GetMouseX(), GetMouseY()), true, 0, NULL);
		}
		for (size_t i = 0; i < wires_len; i++) {
			// NOTE: segfault after wires_len==WIRE_LEN
			if (wires[i].status == Creating) {
				wire_create(&wires[i]);
			} else if (wires[i].status == Editing) {
				// TODO: implement
			}
		}

		// Drawing
		BeginDrawing();
		ClearBackground(BG_COLOUR);
		draw_grid();
		for (size_t i = 0; i < wires_len; i++) {
			wire_draw(&wires[i]);
		}

		EndDrawing();
	}
	CloseWindow();
	return EXIT_SUCCESS;
}
