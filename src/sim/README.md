# Simpletron Simulator

This is the Simpletron simulator program. It implements the Simpletron Machine Language instruction set.
All instructions are written as words. The top half of the word represents the instruction. The bottom
half represents the destination or argument.  The original specification had the following operations.

* 10xx -- READ [Read a number and save at location xx in core memory]
* 11xx -- WRITE [Print the number found in core memory xx]
* 20xx -- LOAD [Load the number found in core memory at xx to the accumulator]
* 21xx -- STORE [Save the value in the accumulator to core memory xx]
* 30xx -- ADD [Add the value at core memory xx to the accumulator]
* 31xx -- SUBTRACT [Subtract the value at core memory xx from the accumulator]
* 32xx -- DIVIDE [Divide the accumulator by the value at xx]
* 33xx -- MULTIPLY [Multiply the accumulator by the value at xx]
* 40xx -- BRANCH [Goto xx and continue execution there]
* 41xx -- BRANCHNEG [If the accumulator is negative, goto xx and continue execution there]
* 42xx -- BRANCHZERO [If the accumulator is 0, goto xx and continue execution there]
* 43xx -- HALT [Stop all execution]

As I have extended the memory, all addresses are now 3 digits.  So 1010 on the original machine
would be 10010 basically, 10xx becomes 10xxx. Aside from that change, all of the original machine
code operations work exactly as specified.

I have extended the Simpletron in the following ways:
* Stack support. You can push (22xxx) and pop (23xxx) values to the stack (which starts at the top of
memory and grows downwards).
* Call and return support. The address after the call (44xxx) instruction is pushed to the stack, when call is
executed. It is popped by the return instruction (45xxx).
* Indirect addressing. If you add 100 to any instruction, it will use the argument as an address to read from.
It will take the value at that address and add the top and bottom halfs to get the read address.
* Increment (35xxx) and Decrement (36xxx) operations (this is especially useful for looping constructs, as you can increment
or decrement the index without saving the accumulator, loading the index, performing the addition or subtraction,
saving the index, and reloading the accumulator).
* Modulus instruction (34xxx), leaves the remainder in the accumulator.
* NOP (00xxx), does nothing (important for extended push/pop as explained below)
* DUMP (01xxx), dump the machine state

Push and Pop are extended to perform two operations.  The lower half of the instruction will be used as an
operator to be performed during the push/pop.  For example, 23031 (POP+SUB will subtract the value from the
top of the stack from the accumulator). This was added because it really simplified code generation for the
LISP compiler.
