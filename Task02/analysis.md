# Task 02 — Return by Reference and Its Dangers

## 1. Objective
Demonstrate how returning a reference to an array element allows direct modification (including assignment on the left-hand side of `=`), capture and explain the compiler warning and undefined behaviour of returning a reference to a local variable, compare `int&`, `const int&`, and returning a copy, analyse `std::vector` reallocation reference invalidation, and compare index and pointer return alternatives.

## 2. Concepts Used
- Return by mutable lvalue reference (`int&`)
- Return by `const` lvalue reference (`const int&`) vs return by value (`int`)
- Dangling reference and undefined behaviour (`-Wreturn-local-addr`)
- `std::vector` dynamic reallocation and iterator/reference/pointer invalidation
- Alternative return types: index (`int`) and pointer (`int*`)

## 3. Explanation of the Code
- [`highest_mark_ref.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task02/highest_mark_ref.cpp): Implements `highestMark`, `readHighestMark`, `copyHighestMark`, and `highestMarkVec`. Demonstrates `highestMark(marks, size) += 2`, `highestMark(examMarks, examSize) = 100`, read-only `const int&`, local copy isolation, and safe address tracking across `std::vector::push_back()` capacity growth.
- [`dangling_reference_demo.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task02/dangling_reference_demo.cpp): Isolates `int& unsafeReference()` returning a local variable to capture GCC's `-Wreturn-local-addr` diagnostic without calling or dereferencing the dangling reference.
- [`highest_mark_alternatives.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task02/highest_mark_alternatives.cpp): Implements `highestIndex`, `highestPointer`, and `highestIndexVec`, tests ties, single-element arrays, empty inputs, and verifies that a saved index remains usable after `std::vector` reallocation.

## 4. Important C++ Rules & Inquiry Answers

### Why Returning a Reference Works on the Left of `=`
A function returning `int&` produces an **lvalue** referring directly to `marks[highestIndex]` inside the caller's array, whose lifetime outlives the function call. Therefore, `highestMark(marks, size) += 2` and `highestMark(examMarks, examSize) = 100` directly read and overwrite that array slot.

### Compiler Warning and Undefined Behaviour (Dangling References)
When `unsafeReference()` returns a reference to automatic variable `localValue`, GCC emits:
`warning: reference to local variable 'localValue' returned [-Wreturn-local-addr]`.
When `unsafeReference()` returns, `localValue`'s stack storage is popped and its lifetime ends. Any subsequent read or write through the returned reference accesses invalid memory and constitutes **undefined behaviour** (it may appear to print stale stack values, crash, or silently corrupt unrelated stack variables under optimization).

### Comparing `int&`, `const int&`, and `int` (Copy)
| Return Type | Caller Can Modify Element? | Lifetime Requirement | Copy Cost |
| :--- | :--- | :--- | :--- |
| `int&` | Yes (valid on LHS of `=`) | Referent must outlive call | None (binds alias) |
| `const int&` | No (read-only view) | Referent must outlive call | None (binds alias) |
| `int` | Modifies only independent copy | None after return | Copies one `int` |

### Changed Condition: Growing `std::vector`
When `vecMarks.push_back(...)` exceeds `vecMarks.capacity()` (observed growing from capacity `3` to `6`), `std::vector` allocates a new, larger contiguous buffer, moves/copies elements into it, and frees the old buffer. As observed in actual execution, `&highest` moved from `0x1a4357fe364` to `0x1a4357fe284`. Any `int&` or `int*` saved before reallocation still points to the deallocated buffer and must never be used.

## 5. Test Cases
- Normal array `{78, 91, 84, 88, 86}` -> `91` moderated to `93`.
- Left-hand-side assignment `{72, 96, 85, 91}` -> `highestMark(...) = 100` sets index `1` to `100`.
- `const int&` vs `int` copy -> adding `5` to local copy (`98`) leaves `marks[1]` at `93`.
- Vector growth from capacity `3` to `6` -> element address changes, proving prior references invalidate.
- Edge cases: ties `{80, 95, 95, 70}` pick first index `1`; single element `{64}` updates to `66`; empty input returns `-1` (index) / `nullptr` (pointer) safely.

## 6. Predicted Output vs Actual Output
All observed results matched `expected_output.txt`. `dangling_reference_demo.cpp` produced the expected `-Wreturn-local-addr` warning and exited cleanly without triggering undefined behaviour.

## 7. Time and Space Complexity
- **Time complexity**: `O(n)` in all three designs (`int&`, `int`, `int*`) for a linear scan of `n` marks.
- **Auxiliary space complexity**: `O(1)` in all three designs (constant index/pointer variables).

## 8. Alternative Approaches & Reallocation Safety
- **Index (`int`)**: Returns `-1` cleanly on empty arrays without throwing exceptions, avoids pointer syntax at the call site (`marks[idx] += 2`), and remains valid after `std::vector` buffer reallocation as long as no earlier elements are inserted or erased.
- **Pointer (`int*`)**: Can return `nullptr` on empty arrays, requires null check and dereferencing (`*ptr += 2`), and like `int&`, is invalidated on `std::vector` reallocation.

## 9. Design Decision and Reason
For a fixed-size C-style array where empty input is prevented (or guarded by exceptions), `int&` gives clean assignment syntax (`highestMark(...) += 2`). Once the register moves to a dynamically growing `std::vector` where callers may retain results across insertions, **returning an index (`int`)** is safer because it handles empty containers cleanly via `-1` and is immune to buffer reallocation.
