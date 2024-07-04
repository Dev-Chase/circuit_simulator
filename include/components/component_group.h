#ifndef COMPONENT_GROUP_H
#define COMPONENT_GROUP_H

#include "action.h"
#ifdef _cplusplus
extern "C" {
#endif

#define DEFAULT_COMPONENT_GROUP_CAPACITY 20
typedef struct ComponentGroup {
  size_t *component_is;
  size_t component_len;
  size_t component_capacity;
} ComponentGroup;

ComponentGroup component_group_new(void);
void component_group_add(ComponentGroup group[static 1], size_t ind);
void component_group_remove(ComponentGroup group[static 1], size_t el);
void component_group_pop(ComponentGroup group[static 1]);
bool component_group_contains(const ComponentGroup group[static 1], size_t ind);
bool component_group_contains_any(const ComponentGroup group[static 1],
                                  const ComponentGroup cmp_group[static 1]);
ComponentGroup
component_get_corresponding(const ComponentGroup group_a[static 1],
                            const ComponentGroup group_b[static 1]);
void component_group_free(ComponentGroup group[static 1]);
void component_group_clear(ComponentGroup group[static 1]);

#ifdef _cplusplus
}
#endif

#endif
