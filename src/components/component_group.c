#include "component_group.h"
#include "circuit.h"
#include "simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComponentGroup component_group_new(void) {
  return (ComponentGroup){
      .component_is = malloc(sizeof(size_t) * DEFAULT_COMPONENT_GROUP_CAPACITY),
      .component_len = 0,
      .component_capacity = DEFAULT_COMPONENT_GROUP_CAPACITY,
  };
}

void component_group_add(ComponentGroup group[static 1], size_t ind) {
  if (group->component_len == group->component_capacity) {
    group->component_capacity *= 2;
    group->component_is = realloc(group->component_is,
                                  sizeof(size_t) * group->component_capacity);

    if (group->component_is == NULL) {
      perror("realloc failure");
      exit(EXIT_FAILURE);
    }
  }

  group->component_is[group->component_len] = ind;
  group->component_len++;
}

void component_group_remove(ComponentGroup group[static 1], size_t el) {
  size_t ind;
  for (ind = 0; ind < group->component_len; ind++) {
    if (group->component_is[ind] == el) {
      break;
    }
  }

  if (ind == group->component_len) {
    fprintf(stderr, "index: %zu\n", ind);
    perror("Invalid Component Passed to component_group_remove");
    exit(EXIT_FAILURE);
  }

  if (ind != group->component_len) {
    memmove(&group->component_is[ind], &group->component_is[ind + 1],
            (group->component_len - ind - 1) * sizeof(size_t));
  }
  group->component_len--;
}

void component_group_pop(ComponentGroup group[static 1]) {
  group->component_len--;
#ifndef NDEBUG
  memset(&group->component_is[group->component_len], 0, sizeof(size_t));
#endif
}

bool component_group_contains(const ComponentGroup group[static 1],
                              size_t ind) {
  for (size_t i = 0; i < group->component_len; i++) {
    if (group->component_is[i] == ind) {
      return true;
    }
  }

  return false;
}

bool component_group_contains_any(const ComponentGroup group[static 1],
                                  const ComponentGroup cmp_group[static 1]) {
  for (size_t i = 0; i < group->component_len; i++) {
    if (component_group_contains(cmp_group, group->component_is[i])) {
      puts("Group Contained any of Cmp Group\n(Selected group contained any of "
           "Hovered)");
      return true;
    }
  }
  return false;
}

// NOTE: only checks group_a num of elements
ComponentGroup
component_get_corresponding(const ComponentGroup group_a[static 1],
                            const ComponentGroup group_b[static 1]) {
  ComponentGroup ret = component_group_new();
  for (size_t i = 0; i < group_a->component_len; i++) {
    if (component_group_contains(group_b, group_a->component_is[i])) {
      component_group_add(&ret, group_a->component_is[i]);
    }
  }

  return ret;
}

void component_group_free(ComponentGroup group[static 1]) {
  free(group->component_is);

#ifndef NDEBUG
  group->component_is = NULL;
  group->component_len = 0;
  group->component_capacity = 0;
#endif
}

void component_group_clear(ComponentGroup group[static 1]) {
  group->component_len = 0;
#ifndef NDEBUG
  memset(group->component_is, 0,
         group->component_capacity * sizeof(Component *));
#endif
}
