# Task 04 — Specifying a Class with an Array Inside It

## 1. Objective
Design and implement a `SavingsAccount` class encapsulating an account number, balance (in integer paise), a fixed-size array of the last 10 transactions, and a transaction count; define some member functions inside the class and others outside using the scope resolution operator (`::`); validate deposits and withdrawals; test the 11th transaction; and compare shifting array, circular buffer, and `std::deque` designs.

## 2. Concepts Used
- Class specification with `private` data invariants and `public` interface
- Inline/in-class member function definitions (`SavingsAccount(...)`, `record()`, `getBalance()`, `getTransactionCount()`)
- Out-of-class member definitions using scope resolution (`SavingsAccount::deposit`, `SavingsAccount::withdraw`, `SavingsAccount::miniStatement`)
- Fixed-size internal array vs circular buffer vs `std::deque`

## 3. Explanation of the Code
- [`savings_account.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task04/savings_account.cpp): Reads operations from [`transactions_input.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task04/transactions_input.txt), validates positive amounts and available balance, records deposits (`+amount`) and withdrawals (`-amount`), and prints the mini-statement at initial state and after valid transactions #10, #11, and #12.
- [`history_alternatives.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task04/history_alternatives.cpp): Runs the same 12 transactions through `ShiftingHistory`, `CircularHistory`, and `DequeHistory` and confirms all three yield the exact same chronological sequence of 10 transactions.

## 4. Important C++ Rules & Inquiry Answers

### Why the Array, Count, and Balance Must Be Private
Keeping `transactions[]`, `count`, and `balancePaise` `private` protects class invariants:
1. External callers cannot mutate `balancePaise` to a negative value or forge transactions without passing through `deposit()` / `withdraw()`.
2. External callers cannot write past `transactions[9]` or corrupt `count` (preventing out-of-bounds buffer corruption and mismatched statement counts).

### What Happens When the 11th Transaction Arrives
- **Shifting fixed array**: `count` is already `10`. Elements `1..9` are copied left by one slot (`transactions[i - 1] = transactions[i]`), discarding index `0` (`+5000`), and the 11th transaction (`-500`) is written to `transactions[9]`. Cost: `k - 1 = 9` element copies.
- **Circular buffer (`start` + `count`)**: Index `(start + count) % 10` points directly to slot `0` (the oldest transaction `+5000`). The new transaction overwrites slot `0` in place and `start` advances to `(start + 1) % 10 = 1`. Cost: zero element shifts (`O(1)`).

### Growing the Requirement from 10 to 50 Transactions
- **Code changes**: When `CAPACITY` is defined as a single `static const int CAPACITY = 10;` constant, changing `10` to `50` requires updating only that constant in both the shifting and circular-buffer classes (and in `std::deque`'s size cap).
- **Runtime scaling**: With shifting, each insertion past capacity moves `49` entries instead of `9` (`5.4x` more element copies per full update). With a circular buffer (or `std::deque`), insertion work remains constant regardless of whether capacity is `10` or `50`.

## 5. Test Cases
- Initial empty account (`SA1001`, opening `100000` paise, 0 transactions).
- Invalid operations: `D 0`, `W -100`, `W 500000` (overdraft) -> all rejected cleanly without modifying balance or history.
- Filling capacity (transactions #1 through #10): balance `111500`, 10 entries from `+5000` to `+1000`.
- Transaction #11 (`W 500`): oldest `+5000` discarded, balance `111000`, window shifts to `-2000 .. -500`.
- Transaction #12 (`D 6000`): oldest `-2000` discarded, balance `117000`, window shifts to `+1500 .. +6000`.

## 6. Predicted Output vs Actual Output
All observed mini-statements and validation messages matched `expected_output.txt`, and all three implementations in `history_alternatives.cpp` matched identically.

## 7. Time and Space Complexity (with Capacity Assumption)
Let `k` be the retained mini-statement capacity (`k = 10`, or `k = 50` after growth) and `n` be the total number of transactions over the account's lifetime:
- **Capacity assumption**: If `k` is treated as a small compile-time constant (`k = 10`), shifting `k - 1 = 9` `long long` values takes bounded constant time (`O(1)` with respect to `n`), though with a higher constant factor. Parameterized by the retained history size `k` (which matters when `k` grows to `50` or larger), insertion when full is `O(k)`.
- **Shifting fixed array**: Insert when full `O(k)` (`k - 1` copies); `miniStatement()` `O(k)`; Space `O(k)` (`80` bytes for `long long[10]` + `4` bytes `count`).
- **Circular fixed array**: Insert `O(1)` (`1` write + modulo); `miniStatement()` `O(k)`; Space `O(k)` (`80` bytes + `start` and `count` integers).
- **`std::deque<long long>`**: Insert `O(1)` amortized (`pop_front()` + `push_back()`); print `O(k)`; Space `O(k)` plus dynamic block/map allocator overhead on the heap.

## 8. Alternative Approaches
Both the circular fixed array and `std::deque` avoid element shifting on the 11th+ transaction. `CircularHistory` achieves `O(1)` insertion using strictly fixed internal storage (zero heap allocations), while `DequeHistory` trades internal heap block allocation for built-in double-ended queue semantics.

## 9. Design Decision and Reason
For an embedded or ATM-style `SavingsAccount` class with a known small bound (`k = 10` or `k = 50`), the **circular fixed array** is the strongest design: it preserves zero-allocation fixed object layout like the original array, scales seamlessly from `k = 10` to `k = 50` without linear shifting overhead, and only adds one `start` index (`4` bytes) and modulo read indexing.
