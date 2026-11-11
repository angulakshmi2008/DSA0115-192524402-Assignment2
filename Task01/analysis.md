# Task 01 — Call by Value, by Reference and by Pointer

## 1. Objective
Implement and compare call by value, call by reference, and call by pointer for a hostel fee concession updater supporting merit (10%), sports (15%), and single-parent (25%) concessions; predict and verify the `change()`/`bigger()`/`show()` program output; complete TODO 1 in the reference concession program; and evaluate the return-by-value alternative and parameter-passing costs.

## 2. Concepts Used
- Call by value (`long long fee`)
- Call by reference (`long long& fee`) and `const` reference (`const Student& s`)
- Call by pointer (`long long* fee`) with `nullptr` guard
- Return by reference (`int& bigger(int& p, int& q)`) used as an lvalue on the left of `=`
- Default arguments evaluated from right to left (`void show(int x, int y = 5, int z = 10)`)

## 3. Explanation of the Code
- [`concession_methods.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task01/concession_methods.cpp): Implements `applyByValue`, `applyByReference`, and `applyByPointer`, then executes all three against the three PDF concessions (10%, 15%, 25%) on a base fee of `45000`, along with `nullptr`, `0%`, invalid `120%`, and sequential concession chaining.
- [`predict_output.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task01/predict_output.cpp): Preserves the exact PDF snippet for output prediction.
- [`complete_todo1.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task01/complete_todo1.cpp): Preserves the PDF code and completes TODO 1 with `fee -= fee * percent / 100;` inside `applyConcession`.
- [`concession_alternative.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task01/concession_alternative.cpp): Implements `applyConcessionReturn(long long fee, int percent)` and compares parameter sizes (`long long` vs a 168-byte `Student` struct) passed by value, `const` reference, and mutable reference.

## 4. Important C++ Rules & Inquiry Answers

### Where Each Parameter Is Stored & Why Only Two Change the Caller's Fee
1. **Call by value (`long long fee`)**: Allocates a separate local parameter variable on the callee's stack frame and copies the caller's value into it. Modifying `fee` changes only that local stack copy; when the function returns, the caller's original variable is untouched (`45000`).
2. **Call by reference (`long long& fee`)**: Binds `fee` directly as an alias to the caller's variable (typically implemented by the compiler via an implicit address without creating a copy of the object). Assignments to `fee` directly modify the caller's storage (`40500`, `38250`, `33750`).
3. **Call by pointer (`long long* fee`)**: Stores a copy of the caller's memory address (`&pointerFee`) in a local pointer parameter. Dereferencing `*fee` accesses and modifies the caller's original memory location. Unlike references, pointers can be `nullptr`, so checking `if (fee == nullptr)` prevents undefined behaviour.

### When to Use a `const` Reference (`const T&`)
Use `const T&` when a function needs read-only access to an object (especially a non-trivial or large object like `Student`) without paying the cost of copying it and without risking accidental mutation. It also binds to temporary values (rvalues) and literals, whereas a non-const reference cannot.

### Predict the Output Analysis (`predict_output.cpp`)
- `change(a, b, &c)`:
  - `a` (by value): local copy becomes `11`, caller `a` stays `1`. (GCC `-Wextra` rightly emits `-Wunused-but-set-parameter` on `a` because the local write is never read.)
  - `b` (by reference): caller `b` becomes `2 + 10 = 12`.
  - `&c` (by pointer): `*c += 10` updates caller `c` to `3 + 10 = 13`.
  - Line 1 printed: `1 12 13`.
- `bigger(a, c) = 100;`:
  - `p` binds to `a` (`1`) and `q` binds to `c` (`13`).
  - `p > q` (`1 > 13`) evaluates to `false`, so `bigger` returns `q` (an `int&` alias to `c`).
  - Assigning `= 100` modifies `c` to `100`, leaving `a` at `1`.
  - Line 2 printed: `1 100`.
  - **Verification correction note**: The prepared Word draft (`CPP_Assignment_2_CO2_Answers.docx`, section 1.3) contained a contradiction: its prose stated *"The final values are a = 1 and c = 100"*, while its output block erroneously printed `100 13`. Step-by-step tracing and actual execution confirm `1 100`.
- `show(int x, int y = 5, int z = 10)`:
  - `show(1)` uses defaults `y = 5, z = 10` -> `1 + 5 + 10 = 16`.
  - `show(1, 2)` uses default `z = 10` -> `1 + 2 + 10 = 13`.
  - `show(1, 2, 3)` supplies all three arguments -> `1 + 2 + 3 = 6`.
  - Line 3 printed: `16 13 6 ` (each call appends a trailing space `' '`).

## 5. Test Cases
- Normal cases: Merit 10% (`45000 -> 40500`), Sports 15% (`45000 -> 38250`), Single Parent 25% (`45000 -> 33750`).
- Sequential stacking: `45000 -> 40500 (10%) -> 34425 (15%) -> 25819 (25% return-by-value)`.
- Boundary & invalid cases: `nullptr` passed to `applyByPointer`, `0%` concession (`45000 -> 45000`), out-of-range `120%` rejected cleanly.

## 6. Predicted Output vs Actual Output
- `predict_output.cpp` matched `prediction.txt` line-for-line (`1 12 13`, `1 100`, `16 13 6 `).
- `complete_todo1.cpp` matched `expected_output.txt` (`after call by value  : 45000`, `after call by reference: 40500`).
- `concession_methods.cpp` matched all pre-run predictions in `prediction.txt`.

## 7. Time and Space Complexity
- **Time complexity**: `O(1)` in all versions (fixed arithmetic operations per call).
- **Space copied per call (measured on 64-bit MinGW-w64 UCRT)**:
  - `long long` (`8` bytes):
    - By value: copies `8` bytes (same size as a pointer/reference on 64-bit; virtually identical cost).
    - By reference / `const` reference: passes an `8`-byte address; no copy of the caller's object beyond binding.
  - `Student` (`168` bytes):
    - By value: copies all `168` bytes (`O(sizeof(Student))` field copy per call).
    - By reference / `const` reference: binds an `8`-byte address without copying the `168`-byte object.

## 8. Alternative Approach (`applyConcessionReturn`)
Returning the revised fee (`long long applyConcessionReturn(long long fee, int percent)`) keeps the caller's original `baseFee` preserved unless explicitly reassigned (`fee = applyConcessionReturn(fee, 10)`), allows clean chaining, and avoids hidden side effects.

## 9. Design Decision and Reason
When **several concessions may apply to one student**, returning the new fee (`long long applyConcessionReturn(long long fee, int percent)`) is preferable to mutating in place when the original base fee must still be audited/retained (for example, to decide whether concessions stack sequentially or add up on the original base fee, or to show "Original Fee vs Net Payable"). If the system only maintains a single mutable account balance field and wants explicit in-place update without forgetting reassignment, call by reference (`void applyConcession(long long& fee, int percent)`) is concise and avoids null-pointer checks.
