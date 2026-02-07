# Quine-McCluskey & Petrick Method Boolean Expression Minimizer

A terminal-based tool to **simplify Boolean expressions** using the **Quine-McCluskey algorithm** and **Petrick's method**. Designed as a lightweight hobby project with an easy-to-use terminal UI.

---

## Features

- Implements **Quine-McCluskey algorithm** for exact minimization of Boolean expressions.  
- Supports **Petrick’s method** for selecting minimal solutions when multiple prime implicants exist.  
- Terminal-based **interactive user interface**.  
- Handles up to `n` variables (customizable).  
- Step-by-step display of:
  - Grouping minterms
  - Reducing minterms
  - Prime implicants
  - Essential prime implicants
  - Column reduction
  - Petrick Algorithm process
  - Final minimized expression

 ## Build Guide
 - Open a terminal and change to the project’s source directory.
  Example: "C:\Users\user123\Desktop\GitHub\Glyphix"

 - Configure the project with CMake:
   Run "cmake -S . -B build"

 - Build the project:
   Run "cmake --build build"

 - After the build completes, quine.exe will be generated in the project directory.
