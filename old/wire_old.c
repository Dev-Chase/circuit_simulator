#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "raylib.h"
#include "wire_old.h"

#include <stdio.h>

bool wire_point_in_line(Wire wire[static 1], size_t i) {
	return (wire->points[i].x == wire->points[i - 1].x ||
			wire->points[i].y == wire->points[i - 1].y);
}

// TODO: find corners that are points and still curve them
void wire_draw(Wire wire[static 1]) {
	for (size_t i = 1; i < wire->len; i++) {
		// if (wire_point_in_line(wire, i)) {
		// }
		DrawLineEx(wire->points[i], wire->points[i - 1], wire->thickness,
				   wire->colour);
		// TODO: draw corners
	}
}

Wire wire_new(Vector2 first_point, bool creating, float thickness, Color *colour) {
	Wire wire = (Wire){
		.thickness = (thickness == 0) ? DEFAULT_WIRE_THICKNESS : thickness,
		.colour = (colour == NULL) ? DEFAULT_WIRE_COLOUR : *colour,
		.points = (Vector2 *)malloc(sizeof(Vector2)),
		.status = (creating) ? Creating : StandBy,
		.len = 1,
	};
	wire.points[0] = first_point;
	return wire;
}

void wire_create(Wire *wire)  {
	if (IsKeyPressed(KEY_ENTER)) {
		wire->status = StandBy;
		return;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		wire_add_point(wire, VEC2(GetMouseX(), GetMouseY()));
	}
}

void wire_add_point(Wire wire[static 1], Vector2 point) {
	wire->len++;
	wire->points = realloc(wire->points, wire->len * sizeof(Vector2));

	if (wire->points == NULL) {
		fprintf(stderr, "Failed to Realloc!");
		exit(EXIT_FAILURE);
	}
wire->points[wire->len - 1] = point;
}

size_t wire_get_point_index(Wire wire[static 1], Vector2 point) {
	for (size_t i = 0; i < wire->len; i++) {
		if (wire->points[i].x == point.x && wire->points[i].y == point.y) {
			return i;
		}
	}

	fprintf(stderr, "Point Doesn't Exist on Wire!");
	exit(EXIT_FAILURE);
}

void wire_del_point(Wire wire[static 1], Vector2 point) {
	size_t point_i = wire_get_point_index(wire, point);
	if (point_i != wire->len - 1) {
		memmove(wire->points + point_i, wire->points + (point_i + 1), (wire->len - point_i - 1) * sizeof(Vector2));
	}

	wire->len--;
	wire->points = realloc(wire->points, sizeof(Vector2) * wire->len);
	if (wire->points == NULL) {
		fprintf(stderr, "Failed to Realloc !");
		exit(EXIT_FAILURE);
	}
}

void wire_move_point(Wire wire[static 1], Vector2 og_pos, Vector2 new_pos) {
	size_t point_i = wire_get_point_index(wire, og_pos);
	wire->points[point_i] = new_pos;	
}
