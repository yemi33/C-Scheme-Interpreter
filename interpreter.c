#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "value.h"
#include "talloc.h"
#include "interpreter.h"
#include "linkedlist.h"
#include "parser.h"
#include "tokenizer.h"

void interpret(Value *tree){
  //create global frame
  Frame *global = NULL;
  global = talloc(sizeof(Frame));
  global -> bindings = makeNull();
  global -> parent = NULL;  

  bind("+", primitiveAdd, global);
  bind("null?", primitiveNull, global);
  bind("car", primitiveCar, global);
  bind("cdr", primitiveCdr, global);
  bind("cons", primitiveCons, global);
  bind("-", primitiveSubtract, global);
  bind("<", primitiveLess, global);
  bind(">", primitiveGreater, global);
  bind("=", primitiveEqual, global);

  while (tree -> type != NULL_TYPE){
    Value *evalTree = eval(car(tree),global);
    displayEval(evalTree);
    tree = cdr(tree);
  }
}

void bind(char *name, Value *(*function)(struct Value *), Frame *frame){
  Value *value = talloc(sizeof(Value));
  value -> type = PRIMITIVE_TYPE;
  value -> pf = function;

  Value *name1 = talloc(sizeof(Value));
  name1 -> type = SYMBOL_TYPE;
  name1 -> s = name; 
  Value *newCons = cons(name1,value);
  frame -> bindings = cons(newCons, frame -> bindings);
}

Value *primitiveSubtract(Value *args){
  Value *firstArg = car(args);
  Value *result = talloc(sizeof(Value));
  
  if (firstArg -> type == INT_TYPE){
    result -> type = INT_TYPE;
    result -> i = firstArg -> i;
  }else if (firstArg -> type == DOUBLE_TYPE){
    result -> type = DOUBLE_TYPE;
    result -> d = firstArg -> d;
  }

  if (args -> type == NULL_TYPE){
    return result;
  }

  args = cdr(args);
  
  while (args -> type != NULL_TYPE){
    Value *item = car(args);
    if (item -> type != DOUBLE_TYPE && item -> type != SYMBOL_TYPE && item -> type != INT_TYPE){
      evaluationError();
    }
    if (item -> type == DOUBLE_TYPE){
      if (result -> type == INT_TYPE){
        double curResultDouble = (double) result -> i;
        curResultDouble = curResultDouble - (item -> d);
        result -> type = DOUBLE_TYPE;
        result -> d = curResultDouble;
      }
      result -> d = (result -> d) - (item -> d);
    }else if (item -> type == INT_TYPE){
      if (result -> type == DOUBLE_TYPE){
        double curItem = (double) item -> i;
        result -> d = (result -> d) - curItem;
      }
      result -> i = (result -> i) - (item -> i);
    }
    args = cdr(args);
  }

  return result;
}

Value *primitiveLess(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 2){
    evaluationError();
  }

  Value *trueExpr = talloc(sizeof(Value));
  trueExpr -> type = BOOL_TYPE;
  trueExpr -> i = 1;
  Value *falseExpr = talloc(sizeof(Value));
  falseExpr -> type = BOOL_TYPE;
  falseExpr -> i = 0;
  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;

  Value *first = car(args);
  Value *second = car(cdr(args));

  if (first -> type == INT_TYPE && second -> type == INT_TYPE){
    if (first -> i < second -> i){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
  
  if (first -> type == DOUBLE_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> d < second -> d){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
    
  if (first -> type == INT_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> i < second -> d){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }

  if (first -> type == DOUBLE_TYPE && second -> type == INT_TYPE){
    if (first -> d < second -> i){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }

  return voidType;
}

Value *primitiveGreater(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 2){
    evaluationError();
  }

  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;

  Value *trueExpr = talloc(sizeof(Value));
  trueExpr -> type = BOOL_TYPE;
  trueExpr -> i = 1;
  Value *falseExpr = talloc(sizeof(Value));
  falseExpr -> type = BOOL_TYPE;
  falseExpr -> i = 0;

  Value *first = car(args);
  Value *second = car(cdr(args));

  if (first -> type == INT_TYPE && second -> type == INT_TYPE){
    if (first -> i > second -> i){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
  
  if (first -> type == DOUBLE_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> d > second -> d){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
    
  if (first -> type == INT_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> i > second -> d){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }

  if (first -> type == DOUBLE_TYPE && second -> type == INT_TYPE){
    if (first -> d > second -> i){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }
  return voidType;
}

Value *primitiveEqual(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 2){
    evaluationError();
  }

  Value *trueExpr = talloc(sizeof(Value));
  trueExpr -> type = BOOL_TYPE;
  trueExpr -> i = 1;
  Value *falseExpr = talloc(sizeof(Value));
  falseExpr -> type = BOOL_TYPE;
  falseExpr -> i = 0;

  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;

  Value *first = car(args);
  Value *second = car(cdr(args));

  if (first -> type == INT_TYPE && second -> type == INT_TYPE){
    if (first -> i == second -> i){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
  
  if (first -> type == DOUBLE_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> d == second -> d){
      return trueExpr;
    }else {
      return falseExpr;
    }
  }
    
  if (first -> type == INT_TYPE && second -> type == DOUBLE_TYPE){
    if (first -> i == second -> d){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }

  if (first -> type == DOUBLE_TYPE && second -> type == INT_TYPE){
    if (first -> d == second -> i){
      return trueExpr;
    } else {
      return falseExpr;
    }
  }
  return voidType;
}

Value *primitiveAdd(Value *args){
  Value *resultInt = talloc(sizeof(Value));
  resultInt -> type = INT_TYPE;
  resultInt -> i = 0;

  Value *resultDouble = talloc(sizeof(Value));
  resultDouble -> type = DOUBLE_TYPE;
  resultDouble -> d = 0.0;

  if (args -> type == NULL_TYPE){
    return resultInt;
  }
  
  while (args -> type != NULL_TYPE){
    Value *item = car(args);
    if (item -> type != DOUBLE_TYPE && item -> type != SYMBOL_TYPE && item -> type != INT_TYPE){
      evaluationError();
    }
    if (item -> type == DOUBLE_TYPE){
      resultDouble -> d = (resultDouble -> d) + (item -> d);
    }else if (item -> type == INT_TYPE){
      resultInt -> i = (resultInt -> i) + (item -> i);
    }
    args = cdr(args);
  }

  if (resultDouble -> d != 0.0){
    if (resultInt -> i != 0){
      double toBeAdded = (double) resultInt -> i;
      resultDouble -> d = (resultDouble -> d) + toBeAdded;
      return resultDouble;
    }else{
      return resultDouble;
    }
  }else{
    return resultInt;
  }
}

Value *primitiveNull(Value *args){
  Value *trueExpr = talloc(sizeof(Value));
  trueExpr -> type = BOOL_TYPE;
  trueExpr -> i = 1;
  Value *falseExpr = talloc(sizeof(Value));
  falseExpr -> type = BOOL_TYPE;
  falseExpr -> i = 0;

  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 1){
    evaluationError();
  }
 
  if (car(args) -> type == CONS_TYPE) {
    if (car(car(args)) -> type == NULL_TYPE){
      return trueExpr;
    }
    if (car(car(car(args))) -> type == NULL_TYPE){
      return trueExpr;
    }else{
      return falseExpr;
    }
  }else{
    return falseExpr;
  }
}

Value *primitiveCar(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 1){
    evaluationError();
  }
 
  if (car(args) -> type != CONS_TYPE){
    evaluationError();
  }

  Value *newCons = cons(car(car(car(args))), makeNull());
  return newCons;
}

Value *primitiveCdr(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 1){
    evaluationError();
  }


  if (car(args) -> type != CONS_TYPE){
    evaluationError();
  }

  Value *newCons = cons(cdr(car(car(args))), makeNull());
  return newCons;
}


Value *primitiveCons(Value *args){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count != 2){
    evaluationError();
  }

  Value *newList = makeNull();
  Value *firstarg = car(args);
  Value *secondarg = car(cdr(args));
  Value *var1 = makeNull();
  Value *var2 = makeNull();

  if (firstarg -> type != CONS_TYPE && secondarg -> type != CONS_TYPE){
    newList = cons(firstarg,secondarg);
    Value *newCons = cons(newList,makeNull());
    return newCons;
  }
  if (firstarg -> type != CONS_TYPE){
    var1 = firstarg;
  }
  else {
    var1 = car(firstarg);
  }

  var2 = car(secondarg);
  newList = cons(var1,var2);
  Value *newCons = cons(newList,makeNull());
  return newCons;

}

Value *eval(Value *tree, Frame *frame){
  Value *evalTree = NULL;
  Value *first = NULL;
  Value *args = NULL;

  //displayEval(tree);
  switch (tree -> type) {
    case INT_TYPE:
      evalTree = tree;
      break;
    case DOUBLE_TYPE:
      evalTree = tree;
      break;
    case STR_TYPE:
      evalTree = tree;
      break;
    case BOOL_TYPE:
      evalTree = tree;
      break;
    case VOID_TYPE:
      break;
    case CLOSURE_TYPE:
      break;
    case SYMBOL_TYPE:
      evalTree = lookUpSymbol(tree,frame);
      //displayEval(evalTree);
      break;
    case PTR_TYPE:
      break;
    case OPEN_TYPE:
      break;
    case CLOSE_TYPE:
      break;
    case NULL_TYPE:
      break;
    case PRIMITIVE_TYPE:
      break;
    case UNSPECIFIED_TYPE:
      break;
    case CONS_TYPE:
      first = car(tree);
      if (cdr(tree) -> type != NULL_TYPE){
        args = cdr(tree);
      }
      else{
        args = NULL;
      }

      if (strcmp(first->s, "if")==0) {
        Value *result = evalIf(args,frame);
        evalTree = result;
      }

      else if (strcmp(first->s, "let")==0){
        Value *result = evalLet(args,frame);
        evalTree = result;
      }

      else if (strcmp(first->s, "let*")==0){
        Value *result = evalLet2(args,frame);
        evalTree = result;
      }
      
      else if (strcmp(first->s, "letrec")==0){
        Value *result = evalLet3(args,frame);
        evalTree = result;
      }
      else if (strcmp(first->s, "begin")==0){
        Value *result = evalBegin(args,frame);
        evalTree = result;
      }
      else if (strcmp(first->s, "set!")==0){
        Value *result = evalSet(args,frame);
        evalTree = result;
      }
      else if (strcmp(first->s, "and")==0){
        Value *result = evalAnd(args,frame);
        evalTree = result;
        //displayEval(evalTree);
      }
      else if (strcmp(first->s, "or")==0){
        Value *result = evalOr(args,frame);
        evalTree = result;
      }

      else if (strcmp(first->s, "quote")==0){
        if (args -> type == CONS_TYPE && cdr(args) -> type != NULL_TYPE){
          evaluationError();
        }
        evalTree = args;
      }

      else if (strcmp(first->s, "define")==0){
        if (args == NULL){
          evaluationError();
        }

        Value *definedFunction = evalDefine(args,frame);
        evalTree = definedFunction;
      }

      else if (strcmp(first->s, "lambda")==0){
        if (args == NULL){
          evaluationError();
        }
        Value *lambdaFunction = evalLambda(args,frame);
        evalTree = lambdaFunction;
      }

      else {
        Value *evaledOperator = eval(first, frame);
        //printf("hello");

        if (args != NULL){
          Value *evaledArgs = evalEach(args, frame);
          evalTree = apply(evaledOperator, evaledArgs);
        }else{
          Value *evaledArgs = makeNull();
          evalTree = apply(evaledOperator, evaledArgs);
        }
      }
      break;
  }
  return evalTree;
}


Value *evalEach(Value *args, Frame *frame){
  Value *listOfEvaled = makeNull();
  while (args -> type != NULL_TYPE){
    Value *arg = car(args);
    Value *evaluated = eval(arg, frame);
    Value *newCons = cons(evaluated,listOfEvaled);
    listOfEvaled = newCons;
    args = cdr(args);
  }
  listOfEvaled = reverse(listOfEvaled);
  return listOfEvaled;
}

Value *apply(Value *function, Value *args){
  if (function -> type == PRIMITIVE_TYPE){
    Value *result = function -> pf (args);
    return result;
  }else{
    Frame *newFrame = talloc(sizeof(Frame)); 
    newFrame -> bindings = makeNull();
    newFrame -> parent = function -> cl.frame;
    Value *param = function -> cl.paramNames;
    while (args -> type != NULL_TYPE){
      Value *arg = car(args);
      Value *formalName = car(car(param));
      Value *newCons = cons(formalName,arg);
      Value *newUpperCons = cons(newCons,newFrame -> bindings);
      newFrame -> bindings = newUpperCons;
      args = cdr(args);
      param = cdr(param);
    }
    return eval(function -> cl.functionCode, newFrame);
  }
}

Value *evalLambda(Value *args, Frame *frame){
  Value *argspointer = NULL; 
  int count = 0;
  argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 2){
    evaluationError();
  }

  Value *closure = talloc(sizeof(Value));
  closure -> type = CLOSURE_TYPE;
  closure -> cl.paramNames = makeNull();
  closure -> cl.functionCode = makeNull();
  
  Value *formalParams = closure -> cl.paramNames;
  Value *functionCode = closure -> cl.functionCode;
  closure -> cl.frame = frame; 

  //looping through bindings
  Value *bindingsInLambda = car(args);
  while (bindingsInLambda -> type != NULL_TYPE){
    if (car(bindingsInLambda) -> type == NULL_TYPE){
      break;
    }
    Value *variableName = car(bindingsInLambda);
    if (variableName -> type != SYMBOL_TYPE){
      evaluationError();
    }
    Value *duplicateChecker = formalParams;
    while (duplicateChecker -> type != NULL_TYPE){
      if (strcmp(car(car(duplicateChecker)) -> s, variableName ->s) == 0){
        evaluationError();
      }
      duplicateChecker = cdr(duplicateChecker);
    }
    Value *newCons = cons(variableName,makeNull());
    Value *newUpperCons = cons(newCons, formalParams);
    formalParams = newUpperCons;
    bindingsInLambda = cdr(bindingsInLambda);
  }
  closure -> cl.paramNames = reverse(formalParams);
  
  //looping through function
  Value *functionInLambda = car(cdr(args));
  functionCode = functionInLambda;
  // while (functionInLambda -> type != NULL_TYPE){
  //     functionCode = functionInLambda;
  //     break;
  // }
  //displayEval(functionCode);

  closure -> cl.functionCode = functionCode;
  //displayEval(closure -> cl.functionCode);
  //printf("\n");
  return closure;
}


Value *evalDefine(Value *args, Frame *frame){
  Value *argspointer = NULL; 
  Value *var = NULL;
  Value *expr = NULL;
  Value *evaluatedExpr = NULL;
  Value *existingBinding = NULL;
  Value *newCons = NULL;
  Value *newBinding = NULL;
  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;
  
  int count = 0;
  argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 2){
    evaluationError();
  }

  if (car(args) -> type != SYMBOL_TYPE){
    evaluationError();
  }
  
  var = car(args);
  expr = car(cdr(args));
  evaluatedExpr = eval(expr, frame);

  existingBinding = frame -> bindings;
  newCons = cons(var,evaluatedExpr);
  newBinding = cons(newCons, existingBinding);
  existingBinding = newBinding;
  frame -> bindings = existingBinding;

  return voidType;
}


Value *lookUpSymbol(Value *tree, Frame *frame){
  Value *var = tree;
  Value *next = frame -> bindings;

  while (next -> type != NULL_TYPE){
    if (strcmp(car(car(next)) -> s,var -> s) == 0){
      return cdr(car(next));
    }
    next = cdr(next);
  }
  
  if (frame -> parent != NULL){
    return lookUpSymbol(tree, frame -> parent);
  }
  else{
    evaluationError(); 
    return makeNull();
  }
}

Value *evalSet(Value *args, Frame *frame){
  Value *var = car(args);
  Value *expr = car(cdr(args));
  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;
  
  while (frame->parent != NULL){
    Value *duplicateChecker = frame -> bindings;
    
    while (duplicateChecker -> type != NULL_TYPE){
      if (strcmp(car(car(duplicateChecker)) -> s, var ->s) == 0){
        
        (duplicateChecker -> c.car) -> c.cdr = eval(expr,frame);
        return voidType;
      }
      duplicateChecker = cdr(duplicateChecker);
    }
    frame = frame -> parent;
  } 
  
  Value *duplicateChecker = frame -> bindings;
  while (duplicateChecker -> type != NULL_TYPE){
    if (strcmp(car(car(duplicateChecker)) -> s, var ->s) == 0){
        
        (duplicateChecker -> c.car) -> c.cdr = eval(expr,frame);
        return voidType;
    }
    duplicateChecker = cdr(duplicateChecker);
  }
  
  return voidType;
}

Value *evalAnd(Value *args, Frame *frame){
  Value *trueResult = talloc(sizeof(Value));
  trueResult -> type = BOOL_TYPE;
  trueResult -> i = 1;

  Value *falseResult = talloc(sizeof(Value));
  falseResult -> type = BOOL_TYPE;
  falseResult -> i = 0;

  Value *result = NULL;
  while (args -> type != NULL_TYPE){
    Value *expr = car(args);
    result = eval(expr,frame);
    if (result -> type != BOOL_TYPE){
      return result;
    }
    if (result -> type == BOOL_TYPE){
      if (result -> i == 1){
        args = cdr(args);
        continue;
      }else if (result -> i == 0){
        return falseResult;
      }else{
        evaluationError();
      }
    } 
  }
  return trueResult;
}

Value *evalOr(Value *args, Frame *frame){
  Value *trueResult = talloc(sizeof(Value));
  trueResult -> type = BOOL_TYPE;
  trueResult -> i = 1;

  Value *falseResult = talloc(sizeof(Value));
  falseResult -> type = BOOL_TYPE;
  falseResult -> i = 0;

  while (args -> type != NULL_TYPE){
    Value *expr = car(args);
    Value *result = eval(expr,frame);
    if (result -> type != BOOL_TYPE){
      return result;
    }
    if (result -> type == BOOL_TYPE){
      if (result -> i == 1){
        //printf("hulu");
        return trueResult;
        break;
      }else if (result -> i == 0){
        args = cdr(args);
        continue;
      }else{
        evaluationError();
      }
    } 
  }
  return falseResult;
}


Value *evalBegin(Value *args, Frame *frame){
  //remember we need to check for if there are no true cases for cond, then return voidType
  Value *voidType = talloc(sizeof(Value));
  voidType -> type = VOID_TYPE;
  Value *result = NULL;
  
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count == 0){
    return voidType;
  }
  while (args -> type != NULL_TYPE){
    result = eval(car(args),frame);
    args = cdr(args);
  }
  return result;
}


Value *evalIf(Value *args, Frame *frame){
  //make sure there are three arguments
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 3){
    evaluationError();
  }

  //evaluate
  Value *test = car(args);
  Value *result = eval(test, frame);
  
  assert(result -> type == BOOL_TYPE);
  if (result -> i == 1){
    Value *trueExpr = car(cdr(args));
    return eval(trueExpr, frame);
  }

  else if (result -> i == 0){
    Value *falseExpr = car(cdr(cdr(args)));
    return eval(falseExpr, frame);
  }

  return makeNull();
}

Value *evalLet3(Value *args, Frame *frame){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 2){
    evaluationError();
  }

  //make a new frame 
  Frame *newFrame = talloc(sizeof(Frame)); 
  newFrame -> bindings = makeNull();
  newFrame -> parent = frame;
  Value *bindingsInLet = car(args);
  
  Value *bodies = cdr(args);

//1. set up variables and assign to some undefined value (undefined type)
//we want to make a new frame for each pair. 

  Value *loop1 = bindingsInLet;
  Value *loop2 = bindingsInLet;

  Value *var;
  Value *val;
  Value *unspecified;
  
  while (loop1 -> type != NULL_TYPE){
    Value *bind = car(loop1);

    if (bind -> type != CONS_TYPE && bind -> type != NULL_TYPE){
      evaluationError();
    }

    var = car(bind);

    //in cases like (let (()) 23), throw error
    if (var -> type == NULL_TYPE){
      evaluationError();
    }

    //in cases like (let ((1 2)) 3), throw error
    if (var -> type != SYMBOL_TYPE){
      evaluationError();
    }

    unspecified = talloc(sizeof(Value));
    unspecified -> type = UNSPECIFIED_TYPE;
    
    Value *newCons = cons(var,unspecified);
    newFrame -> bindings = cons(newCons,newFrame -> bindings);

    loop1 = cdr(loop1);
  }
//2. evaluate the right hand sides and put these in a list
//if any are undefined values, whole letrec returns undefined value (#<undefined>)
  Value *evaluatedVals = makeNull();
  
  while (loop2 -> type != NULL_TYPE){
    Value *bind = car(loop2);

    if (bind -> type != CONS_TYPE && bind -> type != NULL_TYPE){
      evaluationError();
    }

    val = cdr(bind);

    if (val -> type == UNSPECIFIED_TYPE){
      return val;
    }
   
    Value *result = eval(car(val),newFrame);
   
    evaluatedVals = cons(result,evaluatedVals);
    //evaluatedVals = reverse(evaluatedVals);
   
    loop2 = cdr(loop2);
  }

  //displayEval(evaluatedVals);

  //displayEval(evaluatedVals);
//3. assign right hand sides to the symbols on the left hand side 
  Value *bindingCounter = newFrame -> bindings;
  while (bindingCounter -> type != NULL_TYPE){
    Value *binding = car(bindingCounter);
    
    binding -> c.cdr = car(evaluatedVals);
    bindingCounter = cdr(bindingCounter);
    evaluatedVals = cdr(evaluatedVals);
  }

  //displayEval(newFrame -> bindings);

  //if there are multiple bodies, let the executable body be the last one on the list
  while (cdr(bodies) -> type != NULL_TYPE){
    bodies = cdr(bodies);
  }
  //displayEval(bodies);
  Value *body = car(bodies);
  //displayEval(body);
  return eval(body,newFrame);
}

Value *evalLet2(Value *args, Frame *frame){
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 2){
    evaluationError();
  }

  Value *bindingsInLet = car(args);
  Value *bodies = cdr(args);
  
  //if bindings in let* is not pointing a cons type, throw error
  if (bindingsInLet -> type != CONS_TYPE){
    evaluationError();
  }

  //we want to make a new frame for each pair. 
  while (bindingsInLet -> type != NULL_TYPE){
    Frame *newFrame = talloc(sizeof(Frame)); 
    newFrame -> bindings = makeNull();
    newFrame -> parent = frame;

    Value *bind = car(bindingsInLet);

    if (bind -> type != CONS_TYPE && bind -> type != NULL_TYPE){
      evaluationError();
    }

    Value *var = car(bind);
    //displayEval(cdr(bind));
    Value *val = NULL;
    if (cdr(bind) -> type == CONS_TYPE){
      val = eval(car(cdr(bind)),frame);
    }else{
      val = eval(cdr(bind),frame);
    }

    //in cases like (let (()) 23), throw error
    if (var -> type == NULL_TYPE){
      evaluationError();
    }

    //in cases like (let ((1 2)) 3), throw error
    if (var -> type != SYMBOL_TYPE){
      evaluationError();
    }

    Value *newCons = cons(var,val);
    newFrame -> bindings = cons(newCons,newFrame -> bindings);
    frame = newFrame;
    bindingsInLet = cdr(bindingsInLet);
  }

  //if there are multiple bodies, let the executable body be the last one on the list
  while (cdr(bodies) -> type != NULL_TYPE){
    bodies = cdr(bodies);
  }
  Value *body = car(bodies);

  return eval(body,frame);
}


Value *evalLet(Value *args, Frame *frame){
  //make sure there are two arguments
  int count = 0;
  Value *argspointer = args;
  while (argspointer -> type != NULL_TYPE){
    count++;
    argspointer = cdr(argspointer);
  }
  if (count < 2){
    evaluationError();
  }

  //make a new frame 
  Frame *newFrame = talloc(sizeof(Frame)); 
  newFrame -> bindings = makeNull();
  newFrame -> parent = frame;
  Value *bindingsInLet = car(args);
  Value *bodies = cdr(args);

  //if there are multiple bodies, let the executable body be the last one on the list
  while (cdr(bodies) -> type != NULL_TYPE){
    bodies = cdr(bodies);
  }
  Value *body = car(bodies);

  //if bindings in let is not pointint a cons type, throw error
  if (bindingsInLet -> type != CONS_TYPE){
    evaluationError();
  }
  
  //traverse through the bindings in let, create new bindings in the new frame
  while (bindingsInLet -> type != NULL_TYPE){
    Value *bind = car(bindingsInLet);

    if (bind -> type != CONS_TYPE){
      //if the bindings is empty, execute the body
      if (bind -> type == NULL_TYPE){
        return eval(body,newFrame);
      }
      //if the bindings is something other than a cons type or an empty null type, throw error
      evaluationError();
    }

    //in cases like (let (()) 23), throw error
    if (car(bind) -> type == NULL_TYPE){
      evaluationError();
    }

    //in cases like (let ((1 2)) 3), throw error
    if (car(bind) -> type != SYMBOL_TYPE){
      evaluationError();
    }

    //otherwise, evaluate expr
    Value *var = car(bind);
    Value *expr = car(cdr(bind));
    Value *val = eval(expr,frame);
    Value *singlebind = newFrame -> bindings;

    while (singlebind -> type != NULL_TYPE){
      //if the variable name already exists in newFrame's bindings, i.e. (let ((x 1) (x 2)) x), throw error
      if (strcmp(car(car(singlebind)) -> s,var ->s)==0){
        evaluationError();
      }
      singlebind = cdr(singlebind);
    }

    //create a new binding in newFrame
    Value *newCons = cons(var,val);
    Value *newBinding = cons(newCons,newFrame -> bindings);
    newFrame -> bindings = newBinding;

    bindingsInLet = cdr(bindingsInLet);
  }

  //after all the bindings are processed, evaluate body
  return eval(body,newFrame);
}


void evaluationError(){
  printf("Evaluation error\n");
  texit(1);
}


void displayEval(Value *tree){
  Value *entry = tree;

  if (entry -> type == INT_TYPE){
    printf("%i ",entry -> i);
  } else if (entry -> type == DOUBLE_TYPE){
    printf("%f ",entry -> d);
  } else if (entry -> type == STR_TYPE){
    printf("%s ",entry -> s);
  } else if (entry -> type == BOOL_TYPE){
    if (entry -> i == 0){
      printf("#f ");
    }else{
      printf("#t ");
    } 
  } else if (entry -> type == CLOSURE_TYPE){
    printf("#<procedure>");
  //this prints out when the type is unspecified in letrec  
  } else if (entry -> type == UNSPECIFIED_TYPE){
    printf("#<unspecified>");
  } else if (entry -> type == SYMBOL_TYPE){
    printf("%s ",entry -> s);
  } else if (entry -> type == CONS_TYPE){
    Value *carEntry = car(entry);
 
    if (carEntry -> type == CONS_TYPE){
      if (cdr(carEntry) -> type != CONS_TYPE && cdr(carEntry) -> type != NULL_TYPE){
        printf("(");
        displayEval(car(carEntry));
        printf(". ");
        displayEval(cdr(carEntry));
        printf(") ");
      }else{
        printf("(");
        displayEval(carEntry);
        printf(") ");
      }
    //one more case where it's an improper list
    } else if (cdr(entry) -> type != CONS_TYPE && cdr(entry) -> type != NULL_TYPE){
      displayEval(carEntry);
      printf(". ");
    } else {
      displayEval(carEntry);   
    }
    displayEval(cdr(entry));  
  }
}


int isImproper(Value *arg){
  if (arg -> type == CONS_TYPE && cdr(arg) -> type != CONS_TYPE) {
    return 1;
  }else{
    return 0;
  }
}