#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "talloc.h"
#include "parser.h"
#include "linkedlist.h"

// Takes a list of tokens from a Racket program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens){
  Value *stack= makeNull();
  Value *process = makeNull();
  int depth = 0;

  Value *current = tokens;

  while (current->type != NULL_TYPE) {
      Value *token = car(current);
      if (token -> type == CLOSE_TYPE){
        depth--;
        Value *popped = car(stack);
        stack = cdr(stack);
        if (popped-> type == OPEN_TYPE) {
          process = cons(makeNull(), process);
        }
        while (popped-> type != OPEN_TYPE){
          Value *newCons = cons(popped,process);
          process = newCons;
          if (stack -> type != NULL_TYPE){
            popped = car(stack);
            stack = cdr(stack);
          }else{
            break;
          }
        }
        
        Value *newCons = cons(process,stack);
        stack = newCons;
        process = makeNull();
        current = cdr(current);
      }
      else if (token -> type == OPEN_TYPE){
        Value *newCons = cons(token,stack);
        stack = newCons;
        current = cdr(current);
        depth++;
      }else{
        Value *newCons = cons(token,stack);
        stack = newCons;
        current = cdr(current);
      }      
  }

  if (depth != 0) {
    if (depth > 0){
      printf("Syntax error: not enough close parantheses\n");
      texit(1);
    }else if(depth < 0){
      printf("Syntax error: too many close parentheses\n");
      texit(1);
    }
  }
  Value *finalList = makeNull();
  while (stack -> type != NULL_TYPE){
    Value *newCons = cons(car(stack),finalList);
    finalList = newCons;
    stack = cdr(stack);
  }
  return finalList;
}

int isEmpty(Value *head){
  if (head -> type == NULL_TYPE){
    return 1;
  }
  else{
    return 0;
  }
}

// Prints the tree to the screen in a readable fashion. It should look just like
// Racket code; use parentheses to indicate subtrees.
void printTree(Value *token){
  if (token -> type != CONS_TYPE) {
    if (token -> type == INT_TYPE){
      printf("%i ",token -> i);
    } else if (token -> type == DOUBLE_TYPE){
      printf("%f ",token -> d);
    } else if (token -> type == STR_TYPE){
      printf("%s ",token -> s);
    } else if (token -> type == BOOL_TYPE){
      if (token -> i == 0){
        printf("#f ");
      }else{
        printf("#t ");
      } 
    }else if (token -> type == SYMBOL_TYPE){
      printf("%s ",token -> s);
    }
  } else {
    if (car(token) -> type == CONS_TYPE){
      printf("(");
      printTree(car(token));
      printf(") ");
    } else {
      printTree(car(token));
    }
    printTree(cdr(token));
  }
}
  
