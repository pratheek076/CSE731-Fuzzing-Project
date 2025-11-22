CSE 731 Project: Automated Fuzz Testing of cJSON using AFL++

Course: CSE 731: Software Testing (Term I 2025-26)
Submission Deadline: November 25, 2025

1. Team Members & Contributions

Member 1: Pratheek P (MS2025010)
Setup of Docker environment, AFL++ toolchain configuration, and compilation of the target with AddressSanitizer.

Member 2: Shrinithi Andal T (MS2025017)
Development of the harness.c test driver, implementation of the artificial bug for verification, and crash analysis.

2. Project Overview

This project performs Automated Fuzz Testing on the cJSON library to evaluate its robustness against malformed JSON inputs.

Target Application: cJSON — a lightweight C-based JSON parser

Source Repository: https://github.com/DaveGamble/cJSON

Code Size: ~3,000 LOC

Testing Technique: Coverage-guided fuzzing using mutation-based input evolution

The fuzzer continuously mutates seed JSON files and monitors code coverage to discover potential memory corruption bugs, crashes, or unexpected behavior inside the parser.

3. Tools Used

AFL++ (American Fuzzy Lop Plus Plus):
Coverage-guided mutation fuzzer with compile-time instrumentation.

Docker:
Used to containerize and standardize the fuzzing environment.

AddressSanitizer (ASAN):
Detects memory errors such as buffer overflows, use-after-free, and heap corruption.

AI Tool Acknowledgement

As required by the course guidelines, we acknowledge the use of Google Gemini for:

Generating the initial skeleton of harness.c.

Debugging syntax errors in Docker volume mounting commands.

Assisting in structuring the documentation.

4. Directory Structure
/
├── cJSON/              # Complete source code of the target (>1000 LOC)
├── harness.c           # Test harness connecting cJSON to AFL++
├── in/                 # Seed corpus (valid JSON samples)
├── results/            # Screenshots of AFL++ runs and crash analysis
└── README.md           # This documentation file

5. How to Run the Project

Follow the steps below to reproduce the fuzzing environment and results.

Prerequisites

Docker desktop installed on your machine.

Execution Steps
1. Pull the AFL++ Docker Image
docker pull aflplusplus/aflplusplus

2. Start the AFL++ Container

Run this command from the project root directory:
(In MAC)
docker run -ti -v "$(pwd)":/src aflplusplus/aflplusplus

(In Windows Powershell)
docker run -ti -v "${PWD}:/src" aflplusplus/aflplusplus

This mounts your current directory into /src inside the container.

3. Compile cJSON and harness with AFL instrumentation + ASAN

Inside the Docker container:

cd /src
afl-clang-fast -fsanitize=address -o fuzz_target harness.c cJSON/cJSON.c -I cJSON/


This produces the fuzz target executable: fuzz_target

4. Run AFL++
afl-fuzz -i in -o out -- ./fuzz_target


in/ → directory containing initial valid JSON seed files

out/ → output directory for crashes, hangs, queue files, and logs

5. To check crashes
Navigate to out/default/crashes to find the crash id.
- cat id:000000*