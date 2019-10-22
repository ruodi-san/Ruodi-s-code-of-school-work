File name: Testing_README
NetId:rl224

#####################Step1:#####################
command : define and test

#####well-defined functions#####
Eg:
##basic definitions
define (f x y)=(+ x y)
define (f x)=(sin x)
define (MOD x y)=(% x y)
define (g x y)=(pow x y)
##a function contains other defined function
define (f1 x y)=(* x y)
define (f2 x y z)=((f1 x y) z)
define (f3 x y z w)=(*(f2 (+ x y) z w) x)
#change orders in definition
define (f3 x y z w)=(*(f2 (+ x w) z y) x)

###well-defined function which has weird format###
#more than one str in function name or variables
define (fx x y z)=(+(* x y) z)
define (fy ab c d)=(sin (/ ab (- c (log d))))
#have more than one space between id and args
define (f      x1   y z)=(/ (%   x1  z)   y)
#defline has unused parameter, I take this as right input
define (f a b c)=(+ a b)
#numbers appear in rhs defline
define (f a b c)=(/ a (* b (+ c 1)))


#####wrong define input format#####
Eg:
##parenthes mismatch
define (f x y)=(+ x y
define (g x y z)=(/ x(+ y z)
##define with no "=" operator
define (f x y) (+ x y)
##defline empty
define
##redefine a function that is already defined
define (f x y)=(+ x y)
define (f x y)=(* x y)
##define a function which takes no parameter
define(f )=(- a b)
##repeated parameter in lhs defline
define (f x x)=(+ x 1)
##undefined parameter appear in rhs
define (f x y)=(+ x z)

#####test command#####
In test command, I accept three test command input:
#the first argument is a function, the second is a number
#the first argument is a function, the second is a function
#the first argument is a number, and the second is a number

###well-defined test input###
#argument with numerics constant for parameters
test (f 1 2 3) 14.6
test (f 1 2 3) (g 1 2)
#argument call other functions(if these functions are well-defined)
test (f (g 1 2) (g 3 4) (h 5 6 -7)) 99.87
test (f (g 1 2) (g 3 4) (h 5 6 -7)) (g 1 2)
test (f (g 1 2) (g 3 4) (h 5 6 -7)) (g 1 (h 1 2 3))
#both numeric constant in first and second argument
test 1 2
test 3 3

###wrong test input format###
some are similar to wrong define input format:
eg:
#parenthes mismatch
#testline empty

others:
#wrong number of args input
define (f x y z) = (+ (* x y) (- z 3))
test (f 1 2) 1
test (f 1 2 3 4) 1
#wrong number of argument after test command
define (f x y z) = (+ (* x y) (- z 3))
test (f 1 2 3)
test (f 1 2 3) 1 3
#test an undefined function
test (f_undefined 1 2 3) 1
#have unrecognized character on test line
test (f 1 2 a) 1

#####################Step2#####################
command: numint
Eg:
define (f x y) = (+ (* 2 x) y)
numint f 0.001 -1 1 4 5

###wrong numint input format###
#calling for a undefined function
#step size is not a positive number
numint f -1 1 2 4 5
#wrong number of range input
numint f 0.1 1 2 4 5 3 6
numint f 0.1 1 2
#the low range is greater then the high range
numint f 0.1 2 1 4 5
#having more arguments than required
numint f 0.01 -1 1 4 5 a

#####################Step3#####################
command: mcint
Eg:
define (f x y) = (+ (* 2 x) y)
mcint f 1000 -1 1 4 5

###wrong numint input format###
the wrong input format is same as numint
except that the second argument should be a positive integer

#####################Step4#####################
command: max and min
Eg:
define (f x y) = (+ (* 2 x) y)
max f 0.1 0.0001 1 2 3 5 1000
min f 0.1 0.0001 1 2 3 5 1000

###wrong max/min input format###
#calling for a undefined function
#gamma is not a positive number
#convergedDistance is not a positive number
#start point's num doesn't correspond to variables' num
#maximum number of steps is not a positive integer
#having more arguments than required
