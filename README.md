## SML

This is the SML (Simpletron Machine Language) simulator and compiler that
I have created.  It is based on the model suggested by Deital and Deital
in their book.  I have made some extensions to the language and might make
more.

There are 4 programs here:
* sml_sim -- the Simpletron simulator (extended as described below)
* sml_disass -- a disassembler for SML object files, turns the numbers back into machine code
* sml_lisp -- a LISP syntax compiler for the Simpletron
* sml_basic -- a BASIC syntac compiler for the Simpletron

The simulator and disassember are written in C++. They are pretty basic.
You can load a file,
created by hand or with the compiler, by typing its name after the simulator
command. The disassembler takes one argument, a filename for the object file
to be disassembled. There are some problems, like improperly handling bad input and
not respecting or warning about out of value items. But, it functions as
expected according to the book.  See the README.md for each program for specifics about how
it functions.

The compilers are written in C, not C++.  Why?  No real reason, aside from the
fact that I wanted to make sure I still remembered enough of the way C does
things to complete a project in that language.  The BASIC style one follows the
Deital and Deital plan pretty closely. The LISP style one uses more dynamic
memory management strategies and makes a simple AST that it then traverses to
create the machine code.

The project is broken down into the following units:

* sml-files/ -- A collection of misc files used for testing the simulator and compilers. Not all of these will work with the current program. Some are hard-coded for the original version, others are intentially wrong to test the error detection abilities of the compilers.
* src/basiccomp/ -- The BASIC syntax compiler source code (C)
* src/disass/ -- The disassembler source code (C++)
* src/lispcomp/ -- The LISP syntax compiler source code (C)
* src/shared/ -- The shared h file, with definitions common to all programs (C/C++)
* src/sim/ -- The source for the simulator itself (C++)
* src/Makefile -- a very simple Makefile to compile the 4 executable files

Note: The current setup expects 32 bit integers (i.e. 4 byte numbers). In any
case, it must be able to contain the values -999,999 to 999,999.  This is true on 64 bit
machines. On machines with 2 byte integers, you may have to adapt the code.
The original machine (-9999 to 9999) does work on 16 bit integers. You will have
to change some of the code (mainly MEMSIZE and OPFACT).  If you change these values,
you will need to recompile the 4 programs as well as any programs created with the
compilers, or by hand.
