## Quine-McCluskey 
A simple Boolean minterms Expression solver. It takes JSON text input and outputs JSON text with Minimized Expression, grouping and reduction data, 
prime-Implicants, Essential Implicants, Uncovered Minterms, Column Domination and Petrick Process

## Input Struture Example
{"var": 4, "minterms":[1,2,3,4,5,6], "dontCares":[2,3]}
note: dontcares is optional, can be omitted
for CLI, input using pipelining

## Build guide
Prerequisite: emcmake and cmake must be installed and configured beforeHand.
- step1 : Download the zip or clone the repository.
- step2 : Open terminal in the root of the project.
- step3 : Run "emcmake cmake -S . -B build-wasm"     
         The terminal will show 'configaration done' , 'generation done' if completed successfully.
- step4:  Run "cmake --build build-wasm" . 
          The terminal will show progress of compilations.
- After Completing above steps, two files will be created inside the root of the project. - "quine.js" and "quine.wasm" .
