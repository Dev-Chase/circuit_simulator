#ifndef LINE_H
#define LINE_H

#include <stddef.h>
#include "raylib.h"
#ifdef _cplusplus
extern "C" {
#endif

static const float DEFAULT_WIRE_THICKNESS = 2;
#define DEFAULT_WIRE_COLOUR RED

typedef struct Wire {
	Vector2 *points;
	size_t len; // NOTE: len == capacity
	Color colour;
	float thickness;
	enum {StandBy, Editing, Creating} status;
} Wire;

//TODO: switch from raylib coordinates to grid coordinates
void wire_draw(Wire wire[static 1]);
Wire wire_new(Vector2 first_point, bool creating, float thickness, Color *colour);
void wire_create(Wire *wire);

void wire_add_point(Wire wire[static 1], Vector2 point);
size_t wire_get_point_index(Wire wire[static 1], Vector2 point);
void wire_del_point(Wire wire[static 1], Vector2 point);
void wire_move_point(Wire wire[static 1], Vector2 og_pos, Vector2 new_pos);

#ifdef _cplusplus
}
#endif
#endif
