#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

// Create a new NULL_TYPE value node.
Value *makeNull() {
  Value *myNULL = talloc(sizeof(Value));
  myNULL->type = NULL_TYPE;
  return myNULL;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr) {
  Value *myCONS = talloc(sizeof(Value));
  myCONS -> type = CONS_TYPE;
  myCONS -> c.car = newCar;
  myCONS -> c.cdr = newCdr;
  return myCONS;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
  if (list -> type == NULL_TYPE){
    printf("Null\n");
  } else {
    Value *entry = car(list);
    switch (entry -> type) {
      case INT_TYPE:
        printf("This is an INT_TYPE Value: %i\n", entry -> i);
        break;
      case DOUBLE_TYPE:
        printf("This is a DOUBLE_TYPE Value: %f\n", entry -> d);
        break;
      case STR_TYPE:
        printf("This is a STR_TYPE Value: %s\n", entry -> s);
        break;
      case CONS_TYPE:
        printf("This is the car of the CONS_TYPE Value: ");
        display(car(entry));
        break;
      case BOOL_TYPE:
        printf("This is a BOOL_TYPE Value: %i\n", entry -> i);
        break;
      case CLOSE_TYPE:
        printf("This is a CLOSE_TYPE Value: %s\n", entry -> s);
        break;
      case OPEN_TYPE:
        printf("This is an OPEN_TYPE Value: %s\n", entry -> s);
        break;
      case SYMBOL_TYPE:
        printf("This is a SYMBOL_TYPE Value: %s\n", entry -> s);
        break;
      case UNSPECIFIED_TYPE:
        break;
      case NULL_TYPE:
        break;
      case PTR_TYPE:
        break;
      case VOID_TYPE:
        break;
      case CLOSURE_TYPE:
        break;
      case PRIMITIVE_TYPE:
        break;
    }
    list = cdr(list);
    display(list);
  }  
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
Value *reverseHelper(Value *list, Value *newlist){ 
  Value *newCons = NULL;
  Value *newCons2 = NULL;
  Value *newSubList = makeNull();
  Value *newVal = NULL;
  if (isNull(list)){
    return newlist;
  }else{
    Value *entry = car(list);
    switch (entry -> type) {
      case INT_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case DOUBLE_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case STR_TYPE: 
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      //problematic region
      case CONS_TYPE:
        //while (entry -> type != NULL_TYPE){
          //newVal = car(entry);
          //newSubList = cons(newVal, newSubList);
          //entry = cdr(entry);
        //}
        //newlist = cons(newSubList,newlist);
        newVal = car(entry);
        newCons2 = cons(newVal,newSubList);
        newSubList = newCons2;
        newCons = cons(newSubList,newlist);
        newlist = newCons;
        break;
      case NULL_TYPE:
        break;
      case PTR_TYPE:
        break;
      case PRIMITIVE_TYPE:
        break;
      case BOOL_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case CLOSE_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case OPEN_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case SYMBOL_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case UNSPECIFIED_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
      case VOID_TYPE:
        break;
      case CLOSURE_TYPE:
        newCons = cons(entry,newlist);
        newlist = newCons;
        break;
    }
  }
  Value *nextCell = cdr(list);
  return reverseHelper(nextCell,newlist);
}

Value *reverse(Value *list){
  Value *initnewlist = makeNull();
  Value *result = reverseHelper(list, initnewlist);
  return result;
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
  assert(list->type == CONS_TYPE);
  return list->c.car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
  assert(list->type == CONS_TYPE);
  return list->c.cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value){
  if(value->type == NULL_TYPE){
    return true;
  } else {
    return false;
  }
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value){
  //assert(value -> type == CONS_TYPE);
  if (isNull(value)){
    return 0;
  }
  return length(cdr(value)) +1;
}
