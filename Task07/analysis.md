# Task 07 — Diagnose, Redesign and Validate

## 1. Objective
Review Tasks 1–6, diagnose a significant scalability/requirement limitation in one design, state a measurable redesign objective, design and implement a non-library circular-buffer redesign for Q4's `SavingsAccount` transaction history, predict results prior to benchmarking, validate functional equivalence and measure performance across `100,000`, `1,000,000`, and `5,000,000` additions, and critically defend the final design.

## 2. Subsections A–F (Design Challenge Analysis)

### A. Diagnose the Limitation (2 Marks)
Reviewing Tasks 1–6:
- Tasks 1, 2, 3, 5, and 6 already operate in `O(1)` per operation (with Task 6 optimized cleanly via `inline` in headers and Task 2 improved by index return under vector growth).
- **Task 4 (`SavingsAccount` shifting transaction array)** exhibits a structural bottleneck as transaction volume and history capacity grow: once the 10-slot array fills up (on the 11th transaction), every subsequent `deposit()` or `withdraw()` executes a loop shifting `k - 1` (`9`) `long long` values left (`values[i - 1] = values[i]`) just to discard the oldest entry.
- Furthermore, Q4's requirement explicitly raises the possibility of expanding the mini-statement capacity from `10` to `50` transactions, increasing per-insertion copying by over 5x (`49` moves per insert instead of `9`).

### B. Define the Measurable Design Objective (2 Marks)
Redesign the internal transaction history storage without relying on a standard library container (`std::deque`/`std::vector`) so that:
1. **Correctness**: Retain the latest `10` transactions (and support `50` via the `CAPACITY` parameter) in the exact same chronological order as the original shifting design on identical input streams.
2. **Algorithmic complexity**: Reduce insertion time when full from `O(k)` (`k - 1` element shifts) to strict `O(1)` (constant time independent of `k`).
3. **Measurable performance**: Achieve a measurable reduction in insertion wall-clock time across `100,000`, `1,000,000`, and `5,000,000` additions on the same machine, compiler (`GCC 16.2.0`), and optimization flags (`-O0` and `-O2`).

### C. Design the Revised Solution (4 Marks)
Instead of shifting elements left on overflow, store a fixed array `long long values[CAPACITY]`, plus two integer fields: `start` (index of oldest element) and `count` (number of valid elements `0..CAPACITY`).

```text
+--------------------------------------------------+
|          RedesignedCircularHistory               |
+--------------------------------------------------+
| - values : long long[CAPACITY]                   |
| - start  : int   (oldest entry index, init = 0)  |
| - count  : int   (stored entries, 0..CAPACITY)   |
+--------------------------------------------------+
| + add(amount : long long) : void                 |
| + snapshot() const : vector<long long>           |
| + print() const : void                           |
+--------------------------------------------------+
```

**Pseudocode**:
```text
add(amount):
    position <- (start + count) mod CAPACITY
    values[position] <- amount
    if count < CAPACITY:
        count <- count + 1
    else:
        start <- (start + 1) mod CAPACITY

print():
    for i from 0 to count - 1:
        output values[(start + i) mod CAPACITY]
```

**Extra Memory & Trade-offs**:
- Adds `1` extra integer (`start`, `4` bytes) alongside `count`, keeping total overhead `O(1)` (`88` bytes vs `84` bytes for `CAPACITY = 10` on 64-bit).
- Trades contiguous `0..count-1` layout for wrap-around indexing, requiring modulo arithmetic (`% CAPACITY`) when adding or printing in logical order.

### D. Predict Before Implementing (1 Mark)
Saved in [`prediction.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task07/prediction.txt) prior to running code:
- **Functional prediction**: For inputs `100, 200, ..., 1200`, both implementations will output `300 400 500 600 700 800 900 1000 1100 1200`; for Q4's `SA1001` banking sequence, both will end at balance `117000` and retain `+1500 .. +6000`.
- **Performance prediction**: Circular insertion will beat shifting across `100,000`, `1,000,000`, and `5,000,000` additions, with a larger speedup at `-O0` and when `CAPACITY` scales from `10` to `50` (since circular work stays `O(1)` while shifting grows with `k`).

### E. Implement and Validate (2 Marks)
- **Functional equivalence** ([`transaction_validation.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task07/transaction_validation.cpp)):
  - 12-transaction sequence `100..1200`: `PASS` (`300 .. 1200` identical).
  - Q4 `SA1001` banking sequence: `PASS` (Balance `117000`, identical 10 transactions).
  - Boundary counts `0, 1, 9, 10, 11, 25`: `ALL PASS`.
- **Measured Timings** ([`transaction_benchmark.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task07/transaction_benchmark.cpp), 3 trials each, identical verified checksums):

| Configuration & Additions | Shifting Median (ms) | Circular Median (ms) | Speedup |
| :--- | :--- | :--- | :--- |
| `CAP=10`, `-O2`, `100,000` | `0.972` | `0.528` | `1.84x` |
| `CAP=10`, `-O2`, `1,000,000` | `10.835` | `5.335` | `2.03x` |
| `CAP=10`, `-O2`, `5,000,000` | `53.231` | `27.242` | `1.95x` |
| `CAP=50`, `-O2`, `100,000` | `1.631` | `0.485` | `3.36x` |
| `CAP=50`, `-O2`, `1,000,000` | `17.391` | `4.919` | `3.54x` |
| `CAP=50`, `-O2`, `5,000,000` | `88.300` | `25.320` | `3.49x` |
| `CAP=10`, `-O0`, `100,000` | `2.068` | `0.615` | `3.36x` |
| `CAP=10`, `-O0`, `1,000,000` | `21.313` | `7.035` | `3.03x` |
| `CAP=10`, `-O0`, `5,000,000` | `102.826` | `40.020` | `2.57x` |
| `CAP=50`, `-O0`, `100,000` | `8.511` | `0.611` | `13.93x` |
| `CAP=50`, `-O0`, `1,000,000` | `91.537` | `6.628` | `13.81x` |
| `CAP=50`, `-O0`, `5,000,000` | `464.217` | `34.250` | `13.55x` |

### F. Defend the Final Design (2 Marks)
- **Objective achievement**: Achieved. The circular buffer matched 100% of original outputs across all normal and boundary tests, reduced per-insertion work to `O(1)`, and measured **~1.95x faster at `-O2`** (`27.242 ms` vs `53.231 ms` for `5,000,000` insertions at `CAPACITY = 10`) and **~3.49x faster at `-O2`** when scaled to `CAPACITY = 50` (`25.320 ms` vs `88.300 ms`). Crucially, increasing capacity 5x (`10 -> 50`) left circular insertion time flat (`27.2 ms -> 25.3 ms`) while shifting time grew substantially (`53.2 ms -> 88.3 ms`).
- **New costs**: `4` bytes for `start` and modulo indexing inside `add()` and `print()`.
- **When the original shifting design may still be preferable**: When `CAPACITY` is tiny (e.g. `<= 10`), updates happen infrequently (human-paced ATM deposits/withdrawals), and direct contiguous slice access (`values[0..count-1]` without modulo translation) or minimal implementation simplicity is valued over high-frequency throughput.

## 3. Complexity Summary
| Operation | Original Shifting Array | Redesigned Circular Buffer |
| :--- | :--- | :--- |
| Insert before full | `O(1)` | `O(1)` |
| Insert when full | `O(k)` (`k - 1` copies) | `O(1)` |
| Print `k` transactions | `O(k)` | `O(k)` (with `% k` indexing) |
| Storage | `O(k)` (`k` values + `count`) | `O(k)` (`k` values + `start`, `count`) |
