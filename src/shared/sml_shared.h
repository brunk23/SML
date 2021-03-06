#ifndef SML_SHARED_H
#define SML_SHARED_H

#define MEMSIZE 1000
#define MAXOP 100
#define MAXVAL 999999
#define MINVAL -999999
#define OPFACT 1000
#define INPMAX 39		/* max for dynamnic strings */

/*
 * Not all these opcodes were part of the original spec.
 *   Extended Opcodes Include: INC, DEC, MOD, DUMP, NOP
 *   A_opcodes are indirect opcodes. They set a flag
 *   indirect in the machine state. The memory location
 *   pointed at is loaded, high 3 digits are added to the
 *   low 3 digits and that is used as the actual memory
 *   location
 */
enum OPCODES {
  NOP = 0, DUMP,
  READ = 10, WRITE, SREAD, SWRITE,
  LOAD = 20, STORE, PUSH, POP,
  ADD = 30, SUBTRACT, DIVIDE, MULTIPLY, MOD, INC, DEC,
  BRANCH = 40, BRANCHNEG, BRANCHZERO, HALT, CALL, RET,
  A_READ = 110, A_WRITE,
  A_LOAD = 120, A_STORE,
  A_ADD = 130, A_SUBTRACT, A_DIVIDE, A_MULTIPLY, A_MOD,
  A_INC, A_DEC
};

#endif
