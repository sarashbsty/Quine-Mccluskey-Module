## Quine-McCluskey 
A simple Boolean minterms Expression solver. It takes JSON text input and outputs JSON text with Minimized Expression, grouping and reduction data, 
prime-Implicants, Essential Implicants, Uncovered Minterms, Column Domination and Petrick Process

## Input Struture Example
{"var": 4, "minterms":[1,2,3,4,5,6], "dontCares":[2,3]}
note: dontcares is optional, can be omitted
for CLI, input using pipelining
