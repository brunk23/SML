## SML

This is the SML (Simpletron Machine Language) simulator and compiler that
I have created.  It is based on the model suggested by Deital and Deital
in their book.  I have made some extensions to the language and might make
more.

The simulator is written in C++. It is pretty basic. You can load a file,
created by hand or with the compiler, by typing its name after the simulator
command. There are some problems, like improperly handling bad input and
not respecting or warning about out of value items. But, it functions as
expected according to the book.

The compiler is written in C, not C++.  Why?  No real reason, aside from the
fact that I wanted to make sure I still remembered enough of the way C does
things to complete the project in that language.  The lisp syntax compiler
currently supports the following commands:

* ; -- anything after a ; is ignored
* input -- (input variable) -- returns value entered or number of chars entered
* print -- (print (statement)) -- returns value printed or number of chars printed
* setf -- (setf variable (statement) )
* if -- (if (condition) (statement) (statement) )
* inc/dec -- (1+ variable) (1- variable) [DONE]
* loop -- (loop (statements) ... (if (cond) (return)))
* cond -- (< <= >= > = !=)
* operators -- (*, /, %, +, -)


Everything is case-insensitive. It will be forced to lower-case.

The basic compiler uses statements similar to the ones given by Deital and
Deital in their activity recommendations.

Note: The current setup expects 32 bit integers (i.e. 4 byte numbers). In any
case, it must be able to contain -999,999 to 999,999.  This is true on 64 bit
machines. On machines with 2 byte integers, you may have to adapt the code.
The original machine (-9999 to 9999) does work on 16 bit integers. You will have
to change some of the code (mainly MEMSIZE and OPFACT).