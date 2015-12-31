# lisp style code that will compile to SML instructions

The lisp syntax compiler current goal is to support the following commands:

* ; -- anything after a ; is ignored  [DONE]
* input -- (input variable) -- returns value entered or number of chars entered
* print -- (print (statement)) -- returns value printed or number of chars printed [80% done]
* setf -- (setf variable (statement) )  [80% done]
* if -- (if (condition) (statement) (statement) ) [DONE]
* inc/dec -- (1+ variable) (1- variable) [DONE]
* loop -- (loop (statements) ... (if (cond) (return)))
* cond -- (< <= >= > = !=) [DONE]
* operators -- (*, /, %, +, -) [DONE]
* defun -- no arguments, but defines a label that can be called as a statement

Everything is case-insensitive. It will be forced to lower-case. 80% done, above, means
that it works for numbers/variables but not strings.

Example Goals
    (defun y
       (input c)
       (+ a (\* b c)))
    (print (y))


All functions leave their results on the accumulator.

    (+ 5 (- 8 4) 7)

will load 5 and push it, it will load 8, subtract 4, then pushadd that to the stack. Then pop the stack and add 7

    (loop
      (statement)
      (statement)
      (if (condition) (return val)) )

Return executes a branch to the start of the loop
