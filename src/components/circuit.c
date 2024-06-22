#include "circuit.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: implement save, run, render_run, & render
Component circuit_component(Circuit ptr[static 1]) {
	return (Component){
		.ptr = (void *)ptr,

		.add_data = circuit_add_component,
		.del_data = circuit_del_component,
		.clear = circuit_clear,
		.free = circuit_free,

		// .save = circuit_save,
		// .run = circuit_run,
		.save = NULL,
		.run = NULL,

		.render = circuit_render,
		// .render_run = circuit_render_run,
		.render_run = NULL,
	};
}

void circuit_fill_new(Circuit circuit[static 1], const char *path) {
#define DEFAULT_CIRCUIT_COMPONENT_LEN 20
	circuit->path = path,
	circuit->components =
		malloc(sizeof(Component) * DEFAULT_CIRCUIT_COMPONENT_LEN),
	circuit->components_len = DEFAULT_CIRCUIT_COMPONENT_LEN;
	circuit->components_capacity = DEFAULT_CIRCUIT_COMPONENT_LEN;
	circuit->this_component = circuit_component(circuit);
}

void circuit_add_component(const Component circuit_component[static 1],
						   const void *const COMPONENT) {
	Circuit *circuit = (Circuit *)circuit_component->ptr;
	if (circuit->components_len == circuit->components_capacity) {
		circuit->components_capacity *= 2;
		circuit->components =
			realloc(circuit->components, circuit->components_capacity);
		if (circuit->components == NULL) {
			perror("malloc fail!");
			exit(EXIT_FAILURE);
		}
	}

	circuit->components[circuit->components_len++] = *(Component *)COMPONENT;
}

void circuit_del_component(const Component circuit_component[static 1],
						   size_t ind) {
	Circuit *circuit = (Circuit *)circuit_component->ptr;

#ifndef NDEBUG
	memset(&circuit->components[ind], 0, sizeof(circuit->components[ind]));
#endif
	if (circuit->components_len != ind + 1) {
		memmove(&circuit->components[ind], &circuit->components[ind + 1],
				(circuit->components_len - ind - 1) *
					sizeof(circuit->components[ind]));
	}

	circuit->components_len--;
}

void circuit_clear(const Component circuit_component[static 1]) {
	Circuit *circuit = (Circuit *)circuit_component->ptr;
	circuit->components_len = 0;

#ifndef NDEBUG
	memset(circuit->components, 0,
		   circuit->components_len * sizeof(circuit->components[0]));
#endif
}

void circuit_free(const Component circuit_component[static 1]) {
	Circuit *circuit = (Circuit *)circuit_component->ptr;
	free(circuit->components);

#ifndef NDEBUG
	circuit->components = NULL;
	circuit->components_len = 0;
	circuit->components_capacity = 0;
#endif
}

// save, run

void circuit_render(const Component circuit_component[static 1]) {
	DrawRectangle(300, 300, 200, 200, WHITE);
}

// render_run
