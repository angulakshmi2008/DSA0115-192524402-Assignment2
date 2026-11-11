# Task 03 — Default Arguments and Function Overloading

## 1. Objective
Diagnose why the original courier overload set fails to compile with an ambiguous call error, design a clean unambiguous overload set supporting weights in grams (`int`), weights in kilograms (`double`), and `Parcel` objects with optional express delivery and insurance via default arguments, predict and verify overload resolution for six calls (explicitly including `charge(2)` and `charge(2.0f)`), and compare against single-struct and function-template alternatives.

## 2. Concepts Used
- Function overloading and C++17 overload resolution ranking (Exact Match > Promotion > Standard Conversion)
- Default arguments (right-to-left rule)
- `const Parcel&` pass-by-const-reference
- Alternative designs: single parameter struct (`ParcelOptions`) and `if constexpr` function template

## 3. Explanation of the Code
- [`ambiguous_original.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task03/ambiguous_original.cpp): Reproduces the broken overload set from the prompt so GCC 16.2.0 emits the genuine ambiguity diagnostic recorded in [`compiler_error.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task03/compiler_error.txt).
- [`courier_overloads.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task03/courier_overloads.cpp): Implements three unambiguous overloads (`charge(double, bool = false, bool = false)`, `charge(int, bool = false, bool = false)`, and `charge(const Parcel&)`) delegating pricing math to a single `computeAmount` helper. Uses clearly labelled illustrative rates (`Rs. 40/kg`, `+Rs. 30` express, `+Rs. 20` insured; the PDF specifies no official tariff).
- [`courier_alternative.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task03/courier_alternative.cpp): Implements both the single `ParcelOptions` struct (showing easy extension with `fragile`) and `chargeTemplate<WeightT>` using C++17 `if constexpr`.

## 4. Important C++ Rules & Inquiry Answers

### Why the Original Design Is Ambiguous
In the original set:
- `charge(double)`
- `charge(double, bool)`
- `charge(double, bool = false, bool = false)`
When calling `charge(2.0)`, **both** `charge(double)` (takes 1 `double`) and `charge(double, bool = false, bool = false)` (takes 1 `double` and fills 2 defaults) are viable and have an **Exact Match** on the 1st argument. Similarly, `charge(2.0, true)` matches both `charge(double, bool)` and `charge(double, bool = false, bool = false)` equally well. Default arguments do not change the priority of a candidate once it is viable; hence GCC rejects both calls as ambiguous.

### Overload Resolution Ranking
When choosing among viable overloads for an argument, C++ ranks implicit conversion sequences from best to worst:
1. **Exact Match** (identical type, lvalue-to-rvalue, array-to-pointer, qualification adjustment such as `Parcel` -> `const Parcel&`).
2. **Promotion** (integral promotion such as `short`/`char` -> `int`; floating-point promotion `float` -> `double`).
3. **Standard Conversion** (e.g., `float` -> `int`, `int` -> `double`, `unsigned int` -> `int` or `double`).
4. **User-Defined Conversion**, then **Ellipsis (`...`)**.

For `charge(2)`: `2` is `int` -> Exact Match for `charge(int, bool, bool)` (beats `int` -> `double` standard conversion).
For `charge(2.0f)`: `2.0f` is `float` -> `float` -> `double` is a **Promotion** (Rank 2), which beats `float` -> `int` **Standard Conversion** (Rank 3), unambiguously selecting `charge(double, bool, bool)`.
*(Note on boundary input: passing `unsigned int` such as `charge(2u)` would convert to `int` and `double` via Standard Conversions of equal rank and remain ambiguous unless an overload or explicit cast is provided.)*

### Why Default Arguments Must Be Given from Right to Left
C++ matches call arguments **positionally from left to right**. If a parameter in the middle had a default while a parameter to its right did not, a caller could not omit the middle argument while still supplying the trailing argument without ambiguous positional syntax. Therefore, every parameter after the first defaulted parameter must also have a default.

## 5. Test Cases
- Six core calls: `charge(2)`, `charge(2.0f)`, `charge(500)`, `charge(2, true)`, `charge(2.0, false, true)`, `charge(box)`.
- Extra promotion and validation cases: `short(750)` (promotes to `int` -> Rs. 30.00), `charge(2.0)` (Rs. 80.00), `charge(-5.0)` (rejected cleanly -> Rs. 0.00), `charge(0)` (Rs. 0.00).

## 6. Predicted Output vs Actual Output
All six core overload selections (`charge(int, bool, bool)`, `charge(double, bool, bool)`, and `charge(const Parcel&)`) and their computed amounts matched [`prediction.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task03/prediction.txt) exactly, along with the `short(750)` promotion and boundary checks.

## 7. Time and Space Complexity
- **Compile-time overload resolution**: Zero runtime overhead; the compiler binds the exact target function statically during compilation.
- **Runtime charge calculation**: `O(1)` arithmetic and branch work.
- **Parameter-copying cost**: Passing `double` or `int` copies `8` or `4` bytes; passing `const Parcel&` binds an `8`-byte address without copying the struct (`ParcelOptions` measured `16` bytes, while a `const ParcelOptions&` reference passes an `8`-byte pointer internally).

## 8. Alternative Approaches
- **Single `ParcelOptions` struct**: Groups fields together (`weightKg`, `express`, `insured`, `fragile`). Adding new options only requires adding a defaulted struct field rather than extending every overload signature.
- **Function template (`chargeTemplate<WeightT>`)**: Eliminates duplicated numeric overload declarations while using C++17 `if constexpr` to treat integral weights as grams and floating-point weights as kilograms.

## 9. Design Decision and Reason
For a small fixed interface supporting convenient shorthand calls (`charge(500)`, `charge(2.0f)`), the three-overload design (`int`, `double`, `const Parcel&`) with right-to-left defaults keeps common calls brief and completely unambiguous. As options grow beyond two booleans (e.g., `fragile`, `priority`, `cod`), passing a single `ParcelOptions` struct by `const` reference prevents boolean-argument mix-ups (`charge(2.0, true, false)` vs `charge(2.0, false, true)`) and scales cleanly.
