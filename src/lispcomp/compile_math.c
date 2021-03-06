#include <stdio.h>

#include "compiler.h"

/*
 * Handles (+ (- (* (/ (%
 */
int comp_math(struct Cons *head, struct Cons **symtree,
	      int code[MEMSIZE],int OPER) {
  int x;
  struct Cons *arg, *curr;

  if( length(head->cdr ) < 2 ) {
    fprintf(stderr,"(%s ",head->car->string);
    printList(head);
    emessg("Too few arguments to function",1);
  }
  /* We just ignore the head, because we always need
   * 2 + arguments */
  curr = head->cdr;
  
  /* Get the first value into accumulator. If it is a
   * symbol or value, we load it. If it is a list, we
   * execute it, because it will leave its result in the
   * accumulator */
  arg = nth(curr,1);
  arg = cType(arg,symtree);
  if(arg->type == LIST) {
    comp_list(arg,symtree,code);
  } else {
    arg->location = iptr(0);
    arg->resolved = NIL;
    code[iptr(1)] = (LOAD*OPFACT) + NIL;
  }
  
  for( x = 2; x <= length(curr); ++x ) {
    arg = nth(curr,x);
    arg = cType(arg,symtree);
    if(arg->type == LIST) {
      code[iptr(1)] = (PUSH*OPFACT) + NIL;
      comp_list(arg,symtree,code);
      code[iptr(1)] = (POP*OPFACT) + OPER;
    } else {
      arg->location = iptr(0);
      arg->resolved = NIL;
      code[iptr(1)] = (OPER*OPFACT) + NIL;
    }
  }
  
  return 0;
}

int comp_incdec(struct Cons *head, struct Cons **symtree,
		int code[MEMSIZE],int oper) {
  struct Cons *arg, *sym;

  arg = head->cdr;

  sym = inSymTree(arg->car,*symtree);

  if( (length(arg) != head->car->args) || !sym ||
      sym->type != VARIABLE) {
    emessg("You can only use '1+' and '1-' with a single variable",1);
  }

  arg->car->resolved = NIL;
  arg->car->location = iptr(0);
  code[iptr(1)] = (oper*OPFACT) + NIL;

  return 0;
}
