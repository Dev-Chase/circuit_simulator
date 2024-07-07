#include "undo_list.h"
#include "component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UndoProcedure Functions
void undo_proc_free(UndoProc proc[static 1]) {
  if (proc->data == NULL) {
    return;
  }
  proc->expired_fn(proc);
}

// UndoList Functions
UndoList undo_list_new(void) { return (UndoList){.procs = {0}, .len = 0}; }

void undo_list_append(UndoList list[static 1], UndoProc proc) {
  if (list->len == MAX_N_UNDO_FNS) {
    list->len--;
  }
  list->procs[list->len] = proc;
  list->len++;
}

void undo_list_pop(UndoList list[static 1]) {
  if (list->len == 0) {
    perror("undo list empty");
    exit(EXIT_FAILURE);
  }
  list->len--;
#ifndef NDEBUG
  memset(list->procs, 0, list->len);
#endif
}

void undo_list_del_head(UndoList list[static 1]) {
  undo_proc_free(&list->procs[0]);
  memmove(&list->procs[0], &list->procs[1],
          (list->len - 1) * sizeof(Component));

  list->len--;
}

UndoProc undo_list_get_last(UndoList list[static 1]) {
  if (list->len == 0) {
    perror("undo list empty");
    exit(EXIT_FAILURE);
  }
  return list->procs[list->len - 1];
}

void undo_list_clear(UndoList list[static 1]) {
  list->len = 0;
#ifndef NDEBUG
  memset(list->procs, 0, MAX_N_UNDO_FNS);
#endif
}

void undo_list_free(UndoList list[static 1]) {
  for (size_t i = 0; i < list->len; i++) {
    undo_proc_free(&list->procs[i]);
  }
}
