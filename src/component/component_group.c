#include "component_group.h"
#include "component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

ComponentGroup component_group_new(void) {
  return (ComponentGroup){
      .ids = malloc(sizeof(uuid_t) * DEFAULT_COMPONENT_GROUP_CAPACITY),
      .len = 0,
      .capacity = DEFAULT_COMPONENT_GROUP_CAPACITY,
  };
}

void component_group_add(ComponentGroup group[static 1], uuid_t el) {
  if (group->len == group->capacity) {
    group->capacity *= 2;
    group->ids = realloc(group->ids, sizeof(size_t) * group->capacity);

    if (group->ids == NULL) {
      perror("realloc failure");
      exit(EXIT_FAILURE);
    }
  }

  uuid_copy(group->ids[group->len], el);
  group->len++;
}

void component_group_remove(ComponentGroup group[static 1], uuid_t el) {
  size_t ind;
  for (ind = 0; ind < group->len; ind++) {
    if (uuid_compare(group->ids[ind], el) == 0) {
      break;
    }
  }

  if (ind == group->len) {
    fprintf(stderr, "index: %zu\n", ind);
    perror("Invalid Component Passed to component_group_remove");
    exit(EXIT_FAILURE);
  }

  memmove(&group->ids[ind], &group->ids[ind + 1],
          (group->len - ind - 1) * sizeof(uuid_t));
  group->len--;
}

void component_group_pop(ComponentGroup group[static 1]) {
  group->len--;
#ifndef NDEBUG
  uuid_clear(group->ids[group->len]);
#endif
}

bool component_group_contains(const ComponentGroup group[static 1], uuid_t el) {
  for (size_t i = 0; i < group->len; i++) {
    if (uuid_compare(group->ids[i], el) == 0) {
      return true;
    }
  }

  return false;
}

bool component_group_contains_any(const ComponentGroup group[static 1],
                                  const ComponentGroup cmp_group[static 1]) {
  for (size_t i = 0; i < group->len; i++) {
    if (component_group_contains(cmp_group, group->ids[i])) {
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
  for (size_t i = 0; i < group_a->len; i++) {
    if (component_group_contains(group_b, group_a->ids[i])) {
      component_group_add(&ret, group_a->ids[i]);
    }
  }

  return ret;
}

void component_group_free(ComponentGroup group[static 1]) {
  free(group->ids);

#ifndef NDEBUG
  group->ids = NULL;
  group->len = 0;
  group->capacity = 0;
#endif
}

void component_group_clear(ComponentGroup group[static 1]) {
  group->len = 0;
#ifndef NDEBUG
  memset(group->ids, 0, group->capacity * sizeof(Component *));
#endif
}
