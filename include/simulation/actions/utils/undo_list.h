#ifndef UNDO_LIST_H
#define UNDO_LIST_H

#include "action.h"
#ifdef _cplusplus
extern "C" {
#endif

// #define MAX_N_UNDO_FNS 30
#define MAX_N_UNDO_FNS 5 // TODO: change back to 30

typedef struct UndoProcedure UndoProc;
typedef void (*ActionUndoFn)(Simulation *, UndoProc *);
typedef void (*ActionUndoExpiredFn)(UndoProc *);
typedef struct UndoProcedure {
  ActionUndoFn fn;
  ActionUndoExpiredFn expired_fn;
  void *data;
  size_t size;
} UndoProc;

typedef struct UndoList {
  UndoProc procs[MAX_N_UNDO_FNS];
  size_t len;
} UndoList;

// UndoProcedure Functions
void undo_proc_free(UndoProc proc[static 1]);

// UndoList Functions
UndoList undo_list_new(void);
void undo_list_append(UndoList list[static 1], UndoProc proc);
void undo_list_pop(UndoList list[static 1]);
void undo_list_del_head(UndoList list[static 1]);
UndoProc undo_list_get_last(UndoList list[static 1]);
void undo_list_clear(UndoList list[static 1]);
void undo_list_free(UndoList list[static 1]);

#ifdef _cplusplus
}
#endif
#endif
