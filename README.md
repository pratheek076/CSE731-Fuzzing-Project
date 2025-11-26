# CSE 731 Project: Automated Fuzz Testing of cJSON using AFL++

**Course:** CSE 731 --- Software Testing (Term I 2025--26)

------------------------------------------------------------------------

## Team Members & Contributions

| Member               | ID         | Contribution |
|----------------------|------------|--------------|
| **Pratheek P**       | MS2025010  | Setup of Docker environment, AFL++ toolchain configuration, AddressSanitizer-enabled compilation |
| **Shrinithi Andal T**| MS2025017  | Development of `harness.c`, insertion of artificial bug, crash analysis |
------------------------------------------------------------------------
## Github repository

https://github.com/pratheek076/CSE731-Fuzzing-Project

------------------------------------------------------------------------
## Project Overview

This project performs automated fuzz testing on the **cJSON** library to
evaluate its robustness against malformed or adversarial JSON inputs.
The fuzzer mutates seed JSON files, tracks program coverage, and
discovers memory errors, crashes, and unexpected behaviors in the cJSON
parser.

### Target Library

-   **Library:** cJSON (lightweight JSON parser in C)
-   **Source:** https://github.com/DaveGamble/cJSON
-   **Code Size:** \~3,000 LOC
-   **Testing Approach:** Mutation-based, coverage-guided fuzzing using
    **AFL++**

------------------------------------------------------------------------

# Automated Fuzz Testing of cJSON Using AFL++

## 1. Introduction

This project evaluates the robustness, correctness, and memory safety of
the cJSON parser using fuzz testing.

### Tools Used

#### AFL++

A modern coverage-guided fuzzer capable of exploring deep execution
paths using genetic mutation, coverage feedback, and optimized for
speed.

#### AddressSanitizer (ASan)

Enabled during compilation to detect: 
- Buffer overflows
- Out-of-bounds accesses
- Use-after-free
- Double-free
- Memory leaks

------------------------------------------------------------------------

## 2. System Design and Architecture

### 2.1 Project Structure

Components include:

1.  **Source Under Test**\
    Local copy of cJSON.

2.  **Test Harness (`harness.c`)**

    -   Reads input from AFL++
    -   Calls `cJSON_Parse()`
    -   Frees memory
    -   Ensures stability for repeated execution

3.  **Fuzzing Infrastructure**

    -   Seeds: `in/`
    -   Outputs: `out/` (crashes, queue, hangs, logs)

------------------------------------------------------------------------

### 2.2 Harness Implementation

The harness workflow: 
1. Read fuzzed input from stdin
2. Pass buffer to `cJSON_Parse`
3. Free returned structure
4. Exit cleanly

------------------------------------------------------------------------

### 2.3 Compilation Pipeline

Instrumented build:

``` bash
afl-clang-fast -fsanitize=address -g -O0 -o fuzz_target harness.c cJSON/cJSON.c
```

------------------------------------------------------------------------

### 2.4 Fuzzing Workflow

Run AFL++:

``` bash
afl-fuzz -i in -o out -- ./fuzz_target
```

AFL++ repeatedly:
- Picks an input from the queue
- Mutates it (bit flips, splicing, arithmetic changes)
- Executes target with mutated input
- Measures code coverage
- Saves input if new coverage or crash occurs

This continues indefinitely, maximizing coverage and discovering errors.
------------------------------------------------------------------------

## 3. Results and Observations

### 3.1 Execution Behavior

-   Coverage increased steadily.
-   ASan flagged memory issues.
-   Fork-server accelerated executions.

### 3.2 Crashes Identified

Crashes due to: 
- Missing null terminators
- Unexpected EOF
- Deep nesting
- Invalid numeric formats

Stored in:

    out/default/crashes/

### 3.3 Memory Errors Found

ASan logs show: 
- Out-of-bounds accesses
- Heap over-reads
- Null pointer dereferencing

### 3.4 Seed Corpus Expansion

Fuzzer discovered complex JSON structures automatically.

------------------------------------------------------------------------

## 4. How to Run the Project

### Prerequisites

-   Docker Desktop

------------------------------------------------------------------------

### 1. Pull AFL++ Docker Image

``` bash
docker pull aflplusplus/aflplusplus
```

### 2. Start AFL++ Container

**macOS/Linux**

``` bash
docker run -ti --rm -v "$(pwd)":/src aflplusplus/aflplusplus
```

**Windows PowerShell**

``` powershell
docker run -ti --rm -v "${PWD}:/src" aflplusplus/aflplusplus
```

------------------------------------------------------------------------

### 3. Compile with AFL++ + ASan

Inside Docker:

``` bash
cd /src
afl-clang-fast -fsanitize=address -o fuzz_target harness.c cJSON/cJSON.c -I cJSON/
```

------------------------------------------------------------------------

### 4. Run AFL++

``` bash
afl-fuzz -i in -o out -- ./fuzz_target
```

------------------------------------------------------------------------

### 5. View Crashes

``` bash
cd out/default/crashes
cat id:000000*
```

Reproduce:

``` bash
./fuzz_target < out/default/crashes/id:000000*
```

------------------------------------------------------------------------

### Optional: Use Dictionary

``` bash
afl-fuzz -i in -o out -x dicts/crash_trigger.dict -- ./fuzz_target
```

------------------------------------------------------------------------

### Cleanup

``` bash
rm -rf out
rm fuzz_target
```

------------------------------------------------------------------------

## 5. Conclusion

This project demonstrates that **AFL++ + ASan** is a highly effective
pipeline for discovering hidden vulnerabilities in real-world libraries
such as cJSON.\
Automated mutation-based fuzzing uncovered memory errors, crashes, and
corner cases that traditional testing methods often miss.

The same pipeline can be applied broadly to enhance software
**security**, **robustness**, and **reliability**.


## References

- cJSON Library  
  https://github.com/DaveGamble/cJSON  
- AFL++ Documentation  
  https://github.com/AFLplusplus/AFLplusplus  
- AddressSanitizer (ASan) Documentation  
  https://github.com/google/sanitizers  
- Docker Documentation  
  https://docs.docker.com/  
- LLVM/Clang Documentation  
  https://clang.llvm.org/docs/  
- JSON Specification (RFC 8259)  
  https://www.rfc-editor.org/rfc/rfc8259  

---

## Use of Generative AI

Generative AI tools were used in this project for the following purposes:

- Assisting with the setup instructions for the AFL++ fuzzing framework.  
- Generating step-by-step build and execution guidelines for Docker, ASan, and the fuzzing harness.  
- Helping draft detailed documentation, explanations, and clarifications related to fuzzing architecture and methodology.  
- Formatting and producing this README.md report in a clear, structured, and professional format.  

All experimental design, code implementation, harness development, debugging, and crash analysis were performed by the project team. Generative AI was used **only for documentation support and clarity enhancement**.
