#ifndef _INTERPRETER
#define _INTERPRETER

void interpret(Value *tree);

void bind(char *name, Value *(*function)(struct Value *), Frame *frame);
Value *primitiveAdd(Value *args);
Value *primitiveNull(Value *args);
Value *primitiveCar(Value *args);
Value *primitiveCdr(Value *args);
Value *primitiveCons(Value *args);
Value *primitiveSubtract(Value *args);
Value *primitiveLess(Value *args);
Value *primitiveGreater(Value *args);
Value *primitiveEqual(Value *args);

Value *eval(Value *expr, Frame *frame);
Value *evalEach(Value *args, Frame *frame);
Value *apply(Value *function, Value *args);
Value *evalLambda(Value *args, Frame *frame);
Value *evalDefine(Value *args, Frame *frame);
Value *lookUpSymbol(Value *tree, Frame *frame);
Value *evalIf(Value *args, Frame *frame);
Value *evalLet(Value *args, Frame *frame);
Value *evalLet2(Value *args, Frame *frame);
Value *evalLet3(Value *args, Frame *frame);
Value *evalBegin(Value *args, Frame *frame);
Value *evalSet(Value *args, Frame *frame);
Value *evalAnd(Value *args, Frame *frame);
Value *evalOr(Value *args, Frame *frame);
void evaluationError();
void displayEval(Value *tree);
int isImproper(Value *arg);

#endif

