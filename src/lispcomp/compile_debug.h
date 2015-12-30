#ifndef COMPILE_DEBUG_H
#define COMPILE_DEBUG_H

#include "compiler.h"

int printToken(struct Cons *);
char *printType(enum TYPE);
char *printInternalID(int);
int printList(struct Cons *);
int deleteTree(struct Cons *);

#endif
