# CSE 731 Project: Automated Fuzz Testing of cJSON using AFL++

**Course:** CSE 731: Software Testing (Term I 2025-26)

---

## 1. Team Members & Contributions

* **Member 1:** Pratheek P (MS2025010)
    * **Contribution:** Setup of Docker environment, AFL++ toolchain configuration, and compilation of the target with AddressSanitizer.
* **Member 2:** Shrinithi Andal T (MS2025017)
    * **Contribution:** Development of the `harness.c` test driver, implementation of the artificial bug for verification, and crash analysis.

---

## 2. Project Overview

This project performs **Automated Fuzz Testing** on the `cJSON` library to evaluate its robustness against malformed JSON inputs. The fuzzer continuously mutates seed JSON files and monitors code coverage to discover potential memory corruption bugs, crashes, or unexpected behavior inside the parser.

* **Target Application:** `cJSON` — a lightweight C-based JSON parser.
* **Source Repository:** [https://github.com/DaveGamble/cJSON](https://github.com/DaveGamble/cJSON)
* **Code Size:** ~3,000 LOC.
* **Testing Technique:** Coverage-guided fuzzing using mutation-based input evolution.

---

## 3. Tools Used

* **AFL++ (American Fuzzy Lop Plus Plus):** Coverage-guided mutation fuzzer with compile-time instrumentation.
* **Docker:** Used to containerize and standardize the fuzzing environment.
* **AddressSanitizer (ASAN):** Detects memory errors such as buffer overflows, use-after-free, and heap corruption.

---

## 4. Directory Structure

```text
cJSON/              # Complete source code of the target (>1000 LOC)
harness.c           # Test harness connecting cJSON to AFL++
in/                 # Seed corpus (valid JSON samples)
results/            # Screenshots of AFL++ runs and crash analysis
README.md           # This documentation file
```
---

## 5. How to Run the Project
Follow the steps below to reproduce the fuzzing environment and results.

Prerequisites
Docker Desktop installed on your machine.

---

## Execution Steps

1. Pull the AFL++ Docker Image
* docker pull aflplusplus/aflplusplus

2. Start the AFL++ Container Run the appropriate command for your OS from the project root directory to mount the current folder to /src:

3. For macOS / Linux:
* docker run -ti -v "$(pwd)":/src aflplusplus/aflplusplus

4. For Windows (PowerShell):
* docker run -ti --rm -v "$(pwd)":/src aflplusplus/aflplusplus

5. Compile cJSON and harness with AFL instrumentation + ASAN 
Inside the Docker container, run:

* cd /src
* afl-clang-fast -fsanitize=address -o fuzz_target harness.c cJSON/cJSON.c -I cJSON/
    - This produces the fuzz target i.e executable file: fuzz_target

6. Run AFL++

* afl-fuzz -i in -o out -- ./fuzz_target

    - in/ → Directory containing initial valid JSON seed files.
    - out/ → Output directory for crashes, hangs, queue files, and logs.

7. To Check Crashes If a crash is found, navigate to the output directory to inspect the artifact:

Navigate to the crashes folder
* cd out/default/crashes

View the content of the crash file (replace 'id:000...' with actual filename)
* cat id:000000*