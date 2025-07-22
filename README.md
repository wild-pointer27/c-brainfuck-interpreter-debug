# C Brainfuck interpreter with debugging features
Simple Brainfuck interpreter in C with debugging features. I made this project as a beginner-friendly exercise. It supports all Brainfuck operators and nested loops.
## Installation
1. Compile the code: `gcc -o brainfuck brainfuck.c`
2. If necessary, make the binary file executable: `sudo chmod +x brainfuck`
3. Move it to the local bin folder: `sudo mv brainfuck /usr/local/bin/`
4. Run it: `brainfuck code.bf`
## Extra features
- Custom `:` operator that prints the current pointer's position in a window of five elements from the tape.
- Syntax checker that verifies if all the opened square brackets are closed and vice versa. if necessary it prints the lines of the occurring errors.
## Using the operator :
Here's an extremly simple example that shows how to use the debug operator.

```
, Asks for the digit
> +++++ +++++ . Prints the \n character
[-] Clears the current cell

: Shows where the pointer is located in the tape and the values of the cells displaying the ASCII values of the digit 

Sets the first number to its real value

+++ +++

[
<---- ---->-
]

: Prints the section of the tape containing the correct value of the given digit
```
This program converts a digit given in input to its real value.
