#sml_basic

This creates a BASIC style compiler for my sml_sim program.  It takes the following commands:

* rem  -- this starts a remark, the rest of the line is ignored
* input -- read a number from the keyboard
* sinput -- read a string from the keyboard
* print -- print a number to the screen
* sprint -- print a string to the screen
* inc -- increment a variable
* dec -- decrement a variable
* let -- assign a value to a variable, accepts expressions with ()s, *, +, -, /, and % operators
* goto -- go directly to a line number
* if ... goto -- tests if a condition is true and goes to the location pointed at with the goto, if it is true
* call -- call a line-number as a function
* ret -- return from a function
* end -- halt the processor

A simple program might look like the following:

    10 input a
    20 let b = 4 * ( a - 3 )
    30 inc b
    40 call 60
    50 end
    60 print b
    70 ret
