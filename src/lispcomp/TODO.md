# lisp style code that will compile to SML instructions

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