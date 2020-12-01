#include <stdlib.h>
#include "value.h"
#include "talloc.h"

Value *head = NULL;

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
//malloc(sizeof(Value))
void *talloc(size_t size){
  Value *newMal = malloc(size);
  Value *newPointer = malloc(sizeof(Value));
  newPointer->type = PTR_TYPE;
  newPointer->p = newMal;
  Value *newCons = malloc(sizeof(Value));
  newCons->type = CONS_TYPE;
  newCons->c.car = newPointer;
  newCons->c.cdr = head;
  head = newCons;

  return newMal;
}
// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
  if (head == NULL){
    return;
  }
  free(head -> c.car -> p);
  free(head -> c.car);
  Value *remove = head;
  head = head -> c.cdr;
  free(remove);
  tfree();
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
  tfree();
  exit(status);
}
