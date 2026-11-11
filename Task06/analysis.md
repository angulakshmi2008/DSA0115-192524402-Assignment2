# Task 06 — Friend Functions and Inline Functions

## 1. Objective
Complete the `Time` class TODOs for `minutesBetween()` and `earlier()`, verify correctness across same-time, across-noon, reversed-order, and day-boundary cases, evaluate friend functions versus member functions and public getters (plus encapsulation risks of public data members), and benchmark 10,000,000 calls at `-O0` and `-O2`.

## 2. Concepts Used
- `friend` non-member functions with symmetric parameter syntax (`minutesBetween(a, b)`, `earlier(a, b)`)
- Member functions (`a.minutesTo(b)`, `a.isEarlier(b)`) and public getters (`hour()`, `minute()`)
- Encapsulation and invariant protection (`0 <= h <= 23`, `0 <= m <= 59`)
- `inline` semantics (ODR/header linkage permission + optimization hint, not a guarantee)
- Cross-translation-unit (`-O0` vs `-O2`) microbenchmarking

## 3. Explanation of the Code
- [`complete_time.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/complete_time.cpp): Preserves the PDF `Time` class and completes TODO 1 (`minutesBetween`) and TODO 2 (`earlier`), producing `205 false`.
- [`time_tests.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/time_tests.cpp): Tests `09:45` vs `13:10`, identical time `10:30` vs `10:30`, across noon `11:50` vs `12:10`, later departure `14:15` vs earlier arrival `13:45`, boundaries `00:00` vs `23:59`, and `ValidatedTime` range checks.
- [`time_alternatives.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/time_alternatives.cpp): Side-by-side comparison of friend, member, and public-getter designs.
- [`time_outofline.hpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/time_outofline.hpp), [`time_outofline.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/time_outofline.cpp), [`inline_benchmark.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task06/inline_benchmark.cpp): Benchmarks 10,000,000 calls per pair across five call structures under `-O0` and `-O2`, verified by matching checksum `2424062500`.

## 4. Important C++ Rules & Inquiry Answers

### Friend Function vs Member Function vs Public Getters
- **Friend function**: Non-member function granted access to `private` `h` and `m`. Has no implicit `this` pointer; treats both operands symmetrically (`minutesBetween(departure, arrival)`), and preserves encapsulation without exposing getters to the whole codebase.
- **Member function**: Invoked on a left-hand object via `this` (`departure.minutesTo(arrival)`). Works cleanly, though asymmetric syntax (`a.isEarlier(b)`) feels slightly less natural for commutative operations like `minutesBetween`.
- **Public getters**: Keeps `h` and `m` private while allowing any external utility function to read `hour()` and `minute()`. Slightly increases public surface area, though preserves write protection.
- **Why public data members (`public: int h, m;`) weaken encapsulation**: Any caller could assign `t.h = 99; t.m = -50;`, violating time invariants (`0..23`, `0..59`), and changing the internal representation later (e.g., storing a single `int totalMinutesSinceMidnight`) would break every external caller reading `.h` and `.m`.

### What `inline` Actually Means
1. **Language linkage rule**: `inline` allows a function's definition to appear in multiple translation units (such as a shared header) without violating the One Definition Rule (ODR).
2. **Optimization hint**: It permits—but does **not** force—the compiler to expand the function body at call sites. The compiler still decides based on optimization level (`-O0` vs `-O2`), call-site visibility, and code size.
3. Yes, tiny `Time` helpers like `minutesBetween` and `earlier` **should** be `inline` in headers when called millions of times across `.cpp` files so `-O2` can eliminate call/return overhead across translation units.

## 5. Test Cases
- `Time(9, 45)` vs `Time(13, 10)` -> `205`, `earlier(13:10, 09:45) = false`
- `Time(10, 30)` vs `Time(10, 30)` -> `0`, `earlier = false`
- `Time(11, 50)` vs `Time(12, 10)` -> `20`, `earlier = true`
- `Time(14, 15)` vs `Time(13, 45)` -> `30`, `earlier(14:15, 13:45) = false`, `earlier(13:45, 14:15) = true`
- `Time(0, 0)` vs `Time(23, 59)` -> `1439`, `earlier = true`
- Invalid boundaries (`24:00`, `12:60`, `-1:15`) rejected in `ValidatedTime`.

## 6. Predicted Output vs Actual Output
`complete_time.cpp` printed `205 false`, matching the PDF expected output. All hand-verified test pairs matched `expected_output.txt`.

## 7. Time and Space Complexity & Genuine Benchmark Analysis
- **Complexity**: `O(1)` time and `O(1)` space per call in all three designs.
- **Measured 10,000,000-call medians (3 trials, GCC 16.2.0 64-bit UCRT, checksum `2424062500`)**:

| Variant | `-O0` Median (ms) | `-O2` Median (ms) |
| :--- | :--- | :--- |
| 1. Separate-TU (`no inline`) | `251.919` | `49.365` |
| 2. Same-TU (no `inline` keyword) | `307.630` | `40.885` |
| 3. Inline friend functions | `267.903` | `34.328` |
| 4. Inline member functions | `429.183` | `31.566` |
| 5. Inline getter functions | `398.395` | `35.304` |

**Interpretation of real measurements**:
1. **At `-O0`**: Optimization is disabled, so the compiler does **not** inline functions even with the `inline` keyword. Variants that execute more nested un-inlined calls per iteration (member wrapper / getters making 4 extra `hour()`/`minute()` calls) measured slower (~398–429 ms) than direct friend calls (~252–268 ms).
2. **At `-O2`**: All variants sped up roughly **5x–13x**. Notice that **Separate-TU (`49.365 ms`)** remained the slowest at `-O2` because without Link-Time Optimization (`-flto`), the compiler could not inline across `.cpp` boundaries and still executed 20 million `call`/`ret` instructions. In contrast, header-visible **inline friend (`34.328 ms`)**, **inline member (`31.566 ms`)**, and **inline getter (`35.304 ms`)** functions were all inlined down to nearly identical `O(1)` arithmetic. Same-TU without `inline` (`40.885 ms`) was also inlined by GCC at `-O2` because the definition lived in the same translation unit.
3. Absolute timings depend on CPU clock scaling, OS background scheduling, and compiler optimization flags.

## 8. Alternative Approaches
Member functions (`a.minutesTo(b)`) and public getters (`a.hour()`, `a.minute()`) produce identical results and identical optimized `-O2` speed (`~31–35 ms` per 10M pairs) once inlined.

## 9. Design Decision and Reason
Keep `minutesBetween` and `earlier` as **`inline` `friend` functions** (or inline `operator<` / helper functions alongside read-only `hour()`/`minute()` getters if external UI code also needs to display hours and minutes). `friend` functions keep `h` and `m` protected, support natural symmetric `f(a, b)` syntax, and match member/getter performance once compiled with `-O2`.
