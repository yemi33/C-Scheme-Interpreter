#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"


// Read all of the input from stdin, and return a linked list consisting of the tokens.
Value *tokenize(){
    //head of the list of tokens
    Value *list = makeNull();
    char charRead;
    charRead = (char)fgetc(stdin); //current character
    char next; //In a conditional statement, such as an if statement or a while loop, the variable stores the character that makes the conditional false and therefore causes the program to exit the conditional; makes sure that character is taken into account before moving onto the character after it
    char *charArray;
    Value *newCons = NULL; //new Cons cell
    Value *newVal = NULL;
    while (charRead != EOF) {
        // open parentheses
        if (charRead == '(') {
            newVal = talloc(sizeof(Value));
            newVal -> type = OPEN_TYPE;
            charArray = talloc(10*sizeof(char));
            charArray[0] = '(';
            charArray[1] = '\0';
            newVal -> s = charArray;
            list = link(newVal, list);
            charRead = (char)fgetc(stdin);
            continue;

        // close parentheses    
        } else if (charRead == ')') {
            newVal = talloc(sizeof(Value));
            newVal -> type = CLOSE_TYPE;
            charArray = talloc(10*sizeof(char));
            charArray[0] = ')';
            charArray[1] = '\0';
            newVal -> s = charArray;
            list = link(newVal, list);
            charRead = (char)fgetc(stdin);
            continue;

        // boolean    
        } else if (charRead == '#') {
            charRead = (char)fgetc(stdin);
            if (charRead == 't'){
              newVal = talloc(sizeof(Value));
              newVal -> type = BOOL_TYPE;
              newVal -> i = 1;
              list = link(newVal, list);
              charRead = (char)fgetc(stdin);
              continue;
            } else if (charRead == 'f'){
              newVal = talloc(sizeof(Value));
              newVal -> type = BOOL_TYPE;
              newVal -> i = 0;
              list = link(newVal, list);
              charRead = (char)fgetc(stdin);
              continue;
            } else {
              printf("Syntax error: untokenizeable");
              texit(1);
            }

        // strings    
        } else if (charRead == '"'){
            newVal = talloc(sizeof(Value));
            charArray = talloc(301*sizeof(char));
            newVal -> type = STR_TYPE;
            int index = 0;
            charArray[index] = '"';
            next = (char)fgetc(stdin);
            index++;
            while (next != '"' && next != EOF){
              charArray[index] = next;
              index++;
              next = (char)fgetc(stdin);
            }
            charArray[index] = '"';
            charArray[index+1] = '\0'; //end of word
            
            newVal -> s = charArray;
            list = link(newVal, list);
            charRead = next; //setting charRead as the last saved token when exiting the while loop
            charRead = (char)fgetc(stdin); 
            continue;

        // signed number
        } else if (charRead == '-'||charRead == '+'){
          char *ptr; //to use strtol
          newVal = talloc(sizeof(Value));
          charArray = talloc(301*sizeof(char));
          next = (char)fgetc(stdin);
          if (next == ' ' || next == '\n' || next == '(' || next == ')' || next == EOF){ //if there is a space after -, that mean's it's an operator
            newVal -> type = SYMBOL_TYPE;
            charArray[0] = charRead;
            charArray[1] = '\0';
            newVal -> s = charArray;
            list = link(newVal, list);
            charRead = next;
            continue;
          }else{
            newVal -> type = INT_TYPE;
            int index = 0; //counter
            charArray[index] = charRead;
            index++;
            while (next != ' ' && next != '\n' && next != ')' && next != '(' && next != EOF){
              if (next == '.'){
                newVal -> type = DOUBLE_TYPE; //once it encounters a dot, it knows it's a double type
              }
              charArray[index] = next;
              index++;
              next= (char)fgetc(stdin);
            }
            charArray[index] = '\0'; //end of the number

            //assigning the type of number (int/double)
            if (newVal -> type == INT_TYPE){
              long newInt = strtol(charArray,&ptr,10);
              newVal -> i = newInt;
            }else if (newVal -> type == DOUBLE_TYPE){ 
              double newDouble = strtod(charArray, &ptr);
              newVal -> d = newDouble;
            }
            list = link(newVal, list);
            charRead = next;
            continue;
          }
        // unsigned number
        }else if (charRead >= 48 && charRead <= 57){ //from 0-9
          char *ptr = talloc(sizeof(char)); //to use strtol
          newVal = talloc(sizeof(Value));
          charArray = talloc(301*sizeof(char));
          newVal -> type = INT_TYPE;
          int index = 0; //counter
          charArray[index] = charRead;
          next = (char)fgetc(stdin);
          index++;
          while (next != ' ' && next != '\n' && next != ')' && next != '(' && next != EOF){
            if (next == '.'){
              newVal -> type = DOUBLE_TYPE; 
            }
            charArray[index] = next;
            index++;
            next = (char)fgetc(stdin);
          }
          charArray[index] = '\0'; //end of the number

          //assigning the type of number (int/double)
          if (newVal -> type == INT_TYPE){ 
            long newInt = strtol(charArray,&ptr,10);
            newVal -> i = newInt;
          }else if (newVal -> type == DOUBLE_TYPE){
            double newDouble = strtod(charArray, &ptr);
            newVal -> d = newDouble;
          }
          list = link(newVal, list);
          charRead = next; 
          continue;

        // symbols OTHER than + or - that start with a letter (no "" like strings) or other weird symbols can have digits in the subsequent part 
        //ex) 97 - a, 122 - z, 65 - A, 90 -Z in ASCII
        } else if((charRead >= 97 && charRead <= 122) || (charRead >= 65 && charRead <= 90)|| charRead == 33 || (charRead >=36 && charRead <=38) || charRead == 42 || charRead == 47 || charRead == 58 || (charRead >=60 && charRead <=62) || charRead == 63 || charRead == 126 || charRead ==95 || charRead == 94){
          newVal = talloc(sizeof(Value));
          charArray = talloc(301*sizeof(char));
          newVal -> type = SYMBOL_TYPE;
          int index = 0;
          charArray[index] = charRead;
          index++;
          next = (char)fgetc(stdin);
          while (next != ' ' && next != '\n' && next != ')' && next != '(' && next != EOF){
            charArray[index] = next;
            index++;
            next = (char)fgetc(stdin);
          }
          charArray[index] = '\0'; //end of the symbol

          newVal -> s = charArray;
          list = link(newVal, list);
          charRead = next; 
          continue; 

        // comments
        } else if(charRead == ';'){
            next = (char)fgetc(stdin);
            while (next != '\n' && next != EOF){ //check the next character and if it's a newline or an EOF move on.
              next = (char)fgetc(stdin);
            }
            charRead = next;
            continue;
        
        //empty space and new line character
        } else if(charRead == ' ' ||charRead == '\n'){
          charRead = (char)fgetc(stdin);
          continue;
        
        //if the character is none of the above, it is untokenizeable; throw an error
        }else {
          printf("Syntax error: untokenizeable");
          texit(1);
        }
    }
    
    Value *revList = reverse(list);
    return revList;
}

// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list){
  if (list -> type == NULL_TYPE){
    return;
  }else {
    switch (car(list)->type){
      case BOOL_TYPE:
        if (car(list)->i == 0){
          printf("#f:boolean\n");
        }else{
        printf("#t:boolean\n");
        }
        break;
      case INT_TYPE:
        printf("%i:integer\n", car(list)->i);
        break;
      case STR_TYPE:
        printf("%s:string\n", car(list)->s);
        break;
      case DOUBLE_TYPE:
        printf("%f:double\n", car(list)->d);
        break;
      case SYMBOL_TYPE:
        printf("%s:symbol\n", car(list)->s);
        break;
      case OPEN_TYPE:
        printf("%s:open\n", car(list)->s);
        break;
      case CLOSE_TYPE:
        printf("%s:close\n", car(list)->s);
        break;
      case CONS_TYPE:
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
      case UNSPECIFIED_TYPE:
        break;
      }
    displayTokens(cdr(list));
  }
}

Value *link(Value *newVal, Value *list){
  Value *newCons = cons(newVal,list);
  list = newCons;
  return list;
}
