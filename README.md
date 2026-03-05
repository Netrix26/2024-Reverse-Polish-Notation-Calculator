# C++ Expression Evaluator & RPN Converter

This is a custom mathematical expression evaluator created during my 2nd-semester Algorithms and Data Structures course at university. 

Instead of relying on the C++ Standard Library (`std::stack`, `std::vector`), I implemented custom, dynamically allocated linked lists from scratch to handle all stack and queue operations. The program reads standard mathematical equations (infix), converts them into Reverse Polish Notation (postfix), and evaluates them to find the result.

## Features
* **Custom Data Structures:** Built-from-scratch linked lists functioning as stacks and queues.
* **Infix to Postfix Conversion:** Implements parsing logic to convert human-readable math into computer-readable ONP (Reverse Polish Notation).
* **Advanced Operators:** Supports basic arithmetic (`+`, `-`, `*`, `/`), parentheses, and prefix functions like `MIN`, `MAX`, `IF`, and `N` (negation).
* **Recursion:** Seamlessly handles heavily nested equations and embedded functions.
* **Safe Execution:** Includes basic error handling for division by zero.

## How It Works (Sample Input)

The program first expects an integer representing the number of equations to solve. Each equation must be separated by spaces and end with a dot (`.`).

**Input structure:**
```text
N
T T T T T . 
T T T T .
```
* N - Number of equations to process
* T - Tokens (numbers, operators, or functions) separated by spaces
* . - End of the equation


**Sample input:**
```text
9
3 + 1 .
( 2 + 3 ) * 4 .
10 + 2 * 6 - 4 / 2 .
( 10 + 2 ) * ( 6 - 4 ) / 2 .
MAX ( 2 , 8 , 4 ) + MIN ( 10 , 3 ) * 2 .
IF ( 5 , 42 , 0 ) + N ( 10 ) .
MAX ( 2 + 3 , 5 * 2 ) .
IF ( MIN ( 5 , 10 ) , MAX ( 1 , 2 , 3 ) , N ( 5 ) ) .
MAX ( 2 * MIN ( 3 , 4 ) , IF ( 0 - 1 , 10 , 20 ) ) + 5 .
