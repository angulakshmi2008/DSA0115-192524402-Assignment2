# C++ Programming Assignment 2 — CO2

## Student & Repository Details
- **Student Name**: ANGULAKSHMI S.
- **Register Number**: 192524402
- **Department**: Information Technology
- **GitHub Account**: `angulakshmi2008`
- **Repository Name**: `DSA0115-192524402-Assignment2` *(Assignment PDF template uses `DSA01-<RegisterNumber>-Assignment2`; the cloned course repository remote is `DSA0115-192524402-Assignment2`)*
- **GitHub Repository Link**: https://github.com/angulakshmi2008/DSA0115-192524402-Assignment2
- **Compiler & Version**: `g++.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r3) 16.2.0` (64-bit UCRT)
- **C++ Standard**: C++17

---

## Compilation Instructions

Standard compilation command for every independent single-file program:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic source.cpp -o program.exe
```

For Task 06 multi-file benchmark builds:
```bash
g++ -std=c++17 -O0 -Wall -Wextra -pedantic Task06/inline_benchmark.cpp Task06/time_outofline.cpp -o Task06/benchmark_O0.exe
g++ -std=c++17 -O2 -Wall -Wextra -pedantic Task06/inline_benchmark.cpp Task06/time_outofline.cpp -o Task06/benchmark_O2.exe
```

For Task 07 benchmark builds:
```bash
g++ -std=c++17 -O0 -Wall -Wextra -pedantic Task07/transaction_benchmark.cpp -o Task07/benchmark_O0.exe
g++ -std=c++17 -O2 -Wall -Wextra -pedantic Task07/transaction_benchmark.cpp -o Task07/benchmark_O2.exe
```

> **Windows MinGW-w64 Profile-Path Note**: On Windows machines where MinGW-w64 is installed under a user directory containing a space (such as `C:\Users\ANGULAKSHMI SUBBURAJ\...`), `g++` fails at link time on `default-manifest.o` due to an unquoted `*endfile` spec expansion. Use the included [`compile.ps1`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/compile.ps1) helper (without modifying GCC binaries):
> ```powershell
> .\compile.ps1 -UseDefaultOpt Task01\complete_todo1.cpp
> .\compile.ps1 -Opt O2 -Sources Task06\inline_benchmark.cpp,Task06\time_outofline.cpp -Out Task06\benchmark_O2.exe
> ```

---

## Run Instructions by Task

### Task 01 — Call by Value, Reference and Pointer
```powershell
.\compile.ps1 -UseDefaultOpt Task01\concession_methods.cpp; .\Task01\concession_methods.exe
.\compile.ps1 -UseDefaultOpt Task01\predict_output.cpp; .\Task01\predict_output.exe
.\compile.ps1 -UseDefaultOpt Task01\complete_todo1.cpp; .\Task01\complete_todo1.exe
.\compile.ps1 -UseDefaultOpt Task01\concession_alternative.cpp; .\Task01\concession_alternative.exe
```

### Task 02 — Return by Reference and Its Dangers
```powershell
.\compile.ps1 -UseDefaultOpt Task02\highest_mark_ref.cpp; .\Task02\highest_mark_ref.exe
.\compile.ps1 -UseDefaultOpt Task02\dangling_reference_demo.cpp; .\Task02\dangling_reference_demo.exe
.\compile.ps1 -UseDefaultOpt Task02\highest_mark_alternatives.cpp; .\Task02\highest_mark_alternatives.exe
```

### Task 03 — Default Arguments and Function Overloading
```powershell
# Intentional ambiguity error capture (compile-only diagnostic):
g++ -std=c++17 -Wall -Wextra -pedantic -c Task03\ambiguous_original.cpp -o Task03\ambiguous_original.o

# Redesigned overloads & alternatives:
.\compile.ps1 -UseDefaultOpt Task03\courier_overloads.cpp; .\Task03\courier_overloads.exe
.\compile.ps1 -UseDefaultOpt Task03\courier_alternative.cpp; .\Task03\courier_alternative.exe
```

### Task 04 — Class with an Array Inside It
```powershell
.\compile.ps1 -UseDefaultOpt Task04\savings_account.cpp; .\Task04\savings_account.exe
.\compile.ps1 -UseDefaultOpt Task04\history_alternatives.cpp; .\Task04\history_alternatives.exe
```

### Task 05 — Static Members and Arrays of Objects
```powershell
.\compile.ps1 -UseDefaultOpt Task05\token_predict.cpp; .\Task05\token_predict.exe
.\compile.ps1 -UseDefaultOpt Task05\complete_bus_ticket.cpp; .\Task05\complete_bus_ticket.exe
.\compile.ps1 -UseDefaultOpt Task05\bus_ticket_function_demo.cpp; .\Task05\bus_ticket_function_demo.exe
.\compile.ps1 -UseDefaultOpt Task05\bus_ticket_alternatives.cpp; .\Task05\bus_ticket_alternatives.exe
```

### Task 06 — Friend Functions and Inline Functions
```powershell
.\compile.ps1 -UseDefaultOpt Task06\complete_time.cpp; .\Task06\complete_time.exe
.\compile.ps1 -UseDefaultOpt Task06\time_tests.cpp; .\Task06\time_tests.exe
.\compile.ps1 -UseDefaultOpt Task06\time_alternatives.cpp; .\Task06\time_alternatives.exe
.\compile.ps1 -Opt O0 -Sources Task06\inline_benchmark.cpp,Task06\time_outofline.cpp -Out Task06\benchmark_O0.exe; .\Task06\benchmark_O0.exe
.\compile.ps1 -Opt O2 -Sources Task06\inline_benchmark.cpp,Task06\time_outofline.cpp -Out Task06\benchmark_O2.exe; .\Task06\benchmark_O2.exe
```

### Task 07 — Diagnose, Redesign and Validate
```powershell
.\compile.ps1 -UseDefaultOpt Task07\transaction_validation.cpp; .\Task07\transaction_validation.exe
.\compile.ps1 -Opt O0 Task07\transaction_benchmark.cpp -Out Task07\benchmark_O0.exe; .\Task07\benchmark_O0.exe
.\compile.ps1 -Opt O2 Task07\transaction_benchmark.cpp -Out Task07\benchmark_O2.exe; .\Task07\benchmark_O2.exe
```

---

## Task-Wise Result Summary

| Task | Topic | Status | Key Verified Outcome |
| :--- | :--- | :--- | :--- |
| **Task01** | Call by Value, Reference & Pointer | PASS | Merit (10%), Sports (15%), Single Parent (25%) verified; `predict_output.cpp` output `1 12 13` / `1 100` / `16 13 6 ` matched pre-run prediction (correcting the Word draft's `100 13` typo); TODO 1 output `45000` / `40500` matched PDF. |
| **Task02** | Return by Reference & Dangers | PASS | Direct `+2` moderation (`91 -> 93`) and LHS `= 100` verified; `-Wreturn-local-addr` warning captured cleanly without triggering UB; vector reallocation address shift (`0x1a4357fe364 -> 0x1a4357fe284`) observed; index/pointer alternatives tested. |
| **Task03** | Default Arguments & Overloading | PASS | Ambiguity error captured from original code; redesigned overloads resolved all 6 calls cleanly (`charge(2)` -> `int`, `charge(2.0f)` -> `double` via promotion, `charge(box)` -> `const Parcel&`), matching pre-run prediction. |
| **Task04** | Class with Fixed Transaction Array | PASS | `SavingsAccount` validated inputs, stored 10 transactions, and shifted properly on 11th and 12th transactions (`+1500 .. +6000`, balance `117000`); matched circular buffer and `std::deque` alternatives. |
| **Task05** | Static Members & Arrays of Objects | PASS | `token_predict.cpp` matched `101 103 104 4`; `complete_bus_ticket.cpp` matched PDF expected output (`Seat 1..3`, `Waiting`, `Issued: 3`); two-call function persistence and multi-bus `TicketOffice` verified. |
| **Task06** | Friend & Inline Functions | PASS | `complete_time.cpp` produced `205 false`; all boundary tests passed; 10M-call `-O2` benchmark showed inline friend (`34.328 ms` median) faster than separate-TU non-inline (`49.365 ms` median) while matching inline member (`31.566 ms`) and getter (`35.304 ms`). |
| **Task07** | Diagnose, Redesign & Validate | PASS | Circular buffer produced identical histories across 100..1200, banking, and boundary tests (`ALL PASS`); at `-O2` (`5,000,000` insertions), circular achieved `27.242 ms` vs `53.231 ms` shifting (`1.95x` faster) at `CAP=10` and `25.320 ms` vs `88.300 ms` (`3.49x` faster) at `CAP=50`. |

---

## Testing & Benchmark Evidence Summary

- **Pre-execution Predictions**: Saved in `Task01/prediction.txt`, `Task03/prediction.txt`, `Task05/prediction.txt`, and `Task07/prediction.txt` prior to compiling and executing the target programs.
- **Compiler Warnings/Errors**:
  - `Task01/compiler_warnings.txt`: `-Wunused-but-set-parameter` on parameter `a` in verbatim PDF `predict_output.cpp` (expected illustration of call by value).
  - `Task02/compiler_warnings.txt`: `-Wreturn-local-addr` on `unsafeReference()` in isolated `dangling_reference_demo.cpp` (not executed/dereferenced).
  - `Task03/compiler_error.txt`: `call of overloaded 'charge(...)' is ambiguous` on verbatim broken design in `ambiguous_original.cpp`.
  - All completed solutions and alternatives compiled with **0 errors and 0 warnings** under `-std=c++17 -Wall -Wextra -pedantic`.
- **Benchmarks**:
  - `Task06/benchmark_results.txt`: 10,000,000 call pairs across 5 implementations at `-O0` and `-O2` (3 trials each, verified checksum `2424062500`).
  - `Task07/benchmark_results.txt`: 100,000, 1,000,000, and 5,000,000 insertions comparing Shifting vs Circular Buffer for `CAP=10` and `CAP=50` at `-O0` and `-O2` (3 trials each, matching snapshot checksums).

---

## Folder Structure

```text
DSA0115-192524402-Assignment2/
├── .gitignore
├── compile.ps1
├── README.md
├── CPP_Assignment_2_CO2_Final_Report.docx
├── Task01/
│   ├── concession_methods.cpp
│   ├── predict_output.cpp
│   ├── complete_todo1.cpp
│   ├── concession_alternative.cpp
│   ├── prediction.txt
│   ├── expected_output.txt
│   ├── actual_output.txt
│   ├── compiler_warnings.txt
│   └── analysis.md
├── Task02/
│   ├── highest_mark_ref.cpp
│   ├── dangling_reference_demo.cpp
│   ├── highest_mark_alternatives.cpp
│   ├── expected_output.txt
│   ├── actual_output.txt
│   ├── compiler_warnings.txt
│   └── analysis.md
├── Task03/
│   ├── ambiguous_original.cpp
│   ├── courier_overloads.cpp
│   ├── courier_alternative.cpp
│   ├── prediction.txt
│   ├── expected_output.txt
│   ├── actual_output.txt
│   ├── compiler_error.txt
│   └── analysis.md
├── Task04/
│   ├── savings_account.cpp
│   ├── history_alternatives.cpp
│   ├── transactions_input.txt
│   ├── expected_output.txt
│   ├── actual_output.txt
│   └── analysis.md
├── Task05/
│   ├── token_predict.cpp
│   ├── complete_bus_ticket.cpp
│   ├── bus_ticket_function_demo.cpp
│   ├── bus_ticket_alternatives.cpp
│   ├── prediction.txt
│   ├── expected_output.txt
│   ├── actual_output.txt
│   └── analysis.md
├── Task06/
│   ├── complete_time.cpp
│   ├── time_tests.cpp
│   ├── time_alternatives.cpp
│   ├── time_outofline.hpp
│   ├── time_outofline.cpp
│   ├── inline_benchmark.cpp
│   ├── expected_output.txt
│   ├── actual_output.txt
│   ├── benchmark_results.txt
│   └── analysis.md
└── Task07/
    ├── transaction_validation.cpp
    ├── transaction_benchmark.cpp
    ├── prediction.txt
    ├── expected_output.txt
    ├── actual_output.txt
    ├── benchmark_results.txt
    └── analysis.md
```
