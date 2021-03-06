#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compiler.h"

int compileTree(struct Cons *base, int code[MEMSIZE]) {
  struct Cons *symTree;

  symTree = 0;
  
  /* The first pass collects all the symbols and populates
   * the symbol tree, it generates code for everything but
   * defun options, we call comp_list because everything is
   * contained in a (progn ) list */
  comp_list(base,&symTree,code);
  code[iptr(1)] = (HALT*OPFACT) + NIL;

  assign_symbols(symTree,code);
  resolve_symbols(base,symTree,code);

  deleteTree(symTree);
  free(symTree);

  return 0;
}

int comp_list(struct Cons *c, struct Cons **symtree,
	      int code[MEMSIZE]) {
  struct Cons *object, *curr;

  curr = c;
  while( curr ) {
    object = cType(curr->car, symtree);
    
    if( object->type == INTERNAL ) {
      switch (object->ID) {
      case PROGN:
	comp_progn(curr, symtree, code);
	break;
	
      case SUM:
	comp_math(curr, symtree, code,ADD);
	break;

      case DIFFERENCE:
	comp_math(curr, symtree, code, SUBTRACT);
	break;

    case QUOTIENT:
	comp_math(curr, symtree, code, DIVIDE);
	break;

      case PRODUCT:
	comp_math(curr, symtree, code, MULTIPLY);
	break;

    case REMAINDER:
	comp_math(curr, symtree, code, MOD);
	break;
	
      case PRINT:
	comp_print(curr, symtree, code);
	break;

      case SETF:
	comp_setf(curr, symtree, code);
	break;

      case IF:
	comp_if(curr, symtree, code);
	break;

      case EQL:
      case DNE:
      case GT:
      case GTE:
      case LT:
      case LTE:
	comp_cond(curr, symtree, code);
	break;

      case INCM:
	comp_incdec(curr, symtree, code, INC);
	break;

      case DECM:
	comp_incdec(curr, symtree, code, DEC);
	break;
	
      default:
	printf("Unimplemented\n");
	break;
      }
    }
    curr = curr->cdr;
  }
  return 0;
}
    
      
/*
 * Handles (progn
 * This expects a series of lists. It doesn't produce
 * any code by itself.
 */
int comp_progn(struct Cons *head, struct Cons **symtree,
	       int code[MEMSIZE]) {
  struct Cons *curr;
  curr = head->cdr;
 
  if(curr->car->type == CONSTANT) {
    printList(curr);
    emessg("Constant by itself in progn",1);
  }

  while(curr) {
    if(curr->car->type == LIST ) {
      comp_list(curr->car,symtree,code);
    }
    curr = curr->cdr;
  }
  
  return 0;
}

int comp_print(struct Cons *head, struct Cons **symtree,
	     int code[MEMSIZE]) {
  struct Cons *arg, *temp, *curr, *sym;
  int x, y;

  curr = head->cdr;
  
  if( length(curr) != head->car->args ) {
    fprintf(stderr,"(PRINT ");
    printList(curr);
    emessg("Incorrect number of arguments to print",1);
  }

  arg = curr->car;

  if(arg->type == LIST) {
    comp_list(arg,symtree,code);
    /* we need to make a location to save the value in */
    if( !(temp = malloc( sizeof( struct Cons)))) {
      emessg("Failed to create temp.",1);
    }
    temp->car = 0;
    temp->cdr = 0;
    temp->type = VARIABLE;
    temp->location = -1;
    x = getID();
    temp->ID = x;
    temp->resolved = NIL;
    if( !(temp->string = malloc( 10 ))) {
      emessg("Failed to make room for string.",1);
    }

    /*
     * tempID is getID in base 4, printed backwards
     */
    y = 0;
    while( x > 0 ) {
      temp->string[y] = (x % 4) + '0';
      x = x / 4;
      y++;
    }
    temp->string[y] = 0;
    (*symtree) = push(temp, *symtree);
    arg->resolved = NIL;
    if( arg->string ) {
      free(arg->string);
    }
    if( !(arg->string = malloc(strlen(temp->string)+1))) {
      emessg("Couldn't get new string.",1);
    }
    strncpy(arg->string,temp->string,strlen(temp->string)+1);
    arg->location = iptr(0);
    code[iptr(1)] = (STORE*OPFACT) + NIL;
    arg->car->resolved = NIL;
    if( arg->car->string ) {
      free(arg->car->string);
    }
    if( !(arg->car->string = malloc(strlen(temp->string)+1))) {
      emessg("Couldn't get new string.",1);
    }
    strncpy(arg->car->string,temp->string,strlen(temp->string)+1);
    arg->car->location = iptr(0);
    code[iptr(1)] = (WRITE*OPFACT) + NIL;
    sym = temp;
  } else {
    sym = inSymTree(arg,*symtree);

    if( !sym ) {
      if( arg->type == CONSTANT ) {
	/* Constants always added to the symbol tree */
	(*symtree) = push(copy(arg), *symtree);
	sym = inSymTree(arg,*symtree);
      } else {
	emessg("Can't print something that doesn't exist.",1);
      }
    }
    
    if(sym->type == STRING) {
      arg->location = iptr(0);
      arg->resolved = NIL;
      code[iptr(1)] = (SWRITE*OPFACT) + NIL;
    }
  
    if(sym->type == CONSTANT || sym->type == VARIABLE) {
      arg->location = iptr(0);
      arg->resolved = NIL;
      code[iptr(1)] = (WRITE*OPFACT) + NIL;
    }
  }
  return 0;
}

int comp_setf(struct Cons *head, struct Cons **symtree,
	     int code[MEMSIZE]) {
  struct Cons *curr, *arg, *sym;
  enum TYPE type;
  curr = head->cdr;

  if( length(curr) != head->car->args ) {
    fprintf(stderr,"(");
    printList(head);
    emessg("Wrong number of arguments for setf",1);
  }

  arg = nth(curr, 2); 
  
  type = VARIABLE;
  
  sym = inSymTree(arg,*symtree);
  if(arg->type == LIST) {
    type = LIST;
    comp_list(arg,symtree,code);
  } else {
    arg->location = iptr(0);
    arg->resolved = NIL;
  }
  if(arg->type == CONSTANT) {
    if( !sym ) {
      /* Constants always added to the symbol tree */
      (*symtree) = push(copy(arg), *symtree);
      sym = inSymTree(arg,*symtree);
    }
  }
  if(arg->type == SYMBOL && arg->string[0] == '"') {
    /* variable is a string */
    type = STRING;
    if( !sym ) {
      (*symtree) = push(copy(arg), *symtree);
      sym = inSymTree(arg,*symtree);
      sym->type = type;
    }
  }

  if( !sym && arg->type != LIST ) {
    fprintf(stderr,"(");
    printList(*symtree);
    fprintf(stderr,"\n(");
    printList(head);
    emessg("Source is not in symbol tree",1);
  }

  arg = nth(curr, 1);
  arg->type = type;
  if( type == LIST ) {
    arg->type = VARIABLE;
  }
  sym = inSymTree(arg,*symtree);
  if( !sym ) {
    (*symtree) = push(copy(arg), *symtree);
    sym = inSymTree(arg,*symtree);
  }

  if( type == VARIABLE ) {
    code[iptr(1)] = (LOAD*OPFACT) + NIL;
  }
  
  arg->location = iptr(0);
  arg->resolved = NIL;
  code[iptr(1)] = (STORE*OPFACT) + NIL;
  
  return 0;
}
