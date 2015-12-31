#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"


/* This should print the generated lists */
int printList(struct Cons *list) {
  struct Cons *currCons;
  currCons = list;
  if(list) {
    if(list->type == LIST) {
      while( currCons ) {
	if( currCons->car ) {
	  if( currCons->car->type == LIST ) {
	    fprintf(stderr," ( ");
	    printList(currCons->car);
	  } else {
	    if(currCons->car->type == CONSTANT) {
	      fprintf(stderr," #%i ",currCons->car->value);
	    } else {
	      if(currCons->car->string) {
		fprintf(stderr," \"%s\" ",currCons->car->string);
	      } else {
		fprintf(stderr,"[NONE] ");
	      }
	    }
	    fprintf(stderr,":%i ",currCons->car->location);
	  }
	}
	if( currCons->cdr == 0 ) {
	  fprintf(stderr,") ");
	}
	currCons = currCons->cdr;
      }
    }
    return 0;
  }
  return 0;
}

/*
 * This should delete all the memory.
 */
int deleteTree(struct Cons *curr) {

  if( curr->string ) {
    free(curr->string);
    curr->string = 0;
  }
  if( curr->car ) {
    deleteTree(curr->car);
    free(curr->car);
    curr->car = 0;
  }
  if( curr->cdr ) {
    deleteTree(curr->cdr);
    free(curr->cdr);
    curr->cdr = 0;
  }

  return 0;
}

int printToken(struct Cons *curr) {
  printf("Cons ID: %i\n",curr->ID);
  printf("Cons type: %i\n",curr->type);
  return 0;
}

char *printType(enum TYPE type) {
  switch (type) {
  case CONSTANT:
    return "Constant";
    break;

  case FUNCTION:
    return "Function";
    break;

  case INTERNAL:
    return "Internal Function";
    break;

  case VARIABLE:
    return "Variable";
    break;

  case STRING:
    return "String";
    break;

  default:
    break;
  }
  return "Unknown type!";
}

char *printInternalID(int id) {
  char *values[] = {
    "", "SETF", "INPUT", "IF", "PRINT", "INCM", "DECM", "DNE",
    "EQL", "GT", "GTE", "LT", "LTE", "DEFUN", "PROGN", "SUM", "EOL"
  };

  if( id >= 0 && id < 17 ) {
    return values[id];
  }
  return values[0];
}
