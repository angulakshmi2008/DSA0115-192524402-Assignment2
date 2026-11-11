# Task 05 — Static Members and Arrays of Objects

## 1. Objective
Design a `BusTicket` class using a `static` seat counter and capacity limit shared across all objects, handle waiting-list behaviour for arrays exceeding capacity, predict and verify the `Token` output program, complete TODO 1 and TODO 2 in `BusTicket`, examine static persistence when ticket arrays are created inside a function called twice, and compare global counter, static member, and `TicketOffice` designs for multi-bus operation.

## 2. Concepts Used
- `static` data members (`BusTicket::issued`, `Token::next`) and out-of-class definitions (`int BusTicket::issued = 0;`)
- `static` member functions (`BusTicket::count()`, `Token::issued()`) without a `this` pointer
- Arrays of objects with explicit single-argument constructors (`BusTicket trip[4] = { ... };`)
- Automatic storage duration vs static storage duration
- Multi-instance counter encapsulation (`TicketOffice`)

## 3. Explanation of the Code
- [`token_predict.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/token_predict.cpp): Exact PDF `Token` program predicting `101 103 104 4`.
- [`complete_bus_ticket.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/complete_bus_ticket.cpp): Exact PDF `BusTicket` program with TODO 1 (`if (issued < CAPACITY) { ++issued; seat = issued; }`) and TODO 2 (`return issued;`).
- [`bus_ticket_function_demo.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/bus_ticket_function_demo.cpp): Calls `issueBatch(...)` twice, creating a local `BusTicket batch[2]` array in each invocation.
- [`bus_ticket_alternatives.cpp`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/bus_ticket_alternatives.cpp): Compares a global counter against `TicketOffice` objects controlling two independent buses (`BUS-101` capacity `3` and `BUS-202` capacity `2`).

## 4. Important C++ Rules & Inquiry Answers

### Which Members Must Be Static & Out-of-Class Definition
- **Static**: `issued` (total seats allocated so far), `CAPACITY` (`static const int CAPACITY = 3`), and `count()` (lets the transport office query `BusTicket::count()` even when zero `BusTicket` objects exist or without inspecting any individual ticket).
- **Non-static**: `seat` and `name`, because each passenger ticket owns its individual seat number (or `0` for Waiting) and name.
- **Out-of-class definition**: Declaring `static int issued;` inside the class only reserves the name; `int BusTicket::issued = 0;` outside the class allocates the single shared storage and initializes it once before `main()` runs.

### Why a Static Member Function Cannot Use `this` or Non-Static Members
A static member function belongs to the **class**, not to an instance, and can be invoked as `BusTicket::count()` with no object. Consequently, no `this` pointer is passed into `count()`, so it cannot know which object's `seat` or `name` to read unless an object reference/pointer is explicitly passed as a parameter.

### Creating and Initialising an Array of `BusTicket` Objects
Because `BusTicket` defines `explicit BusTicket(const std::string& n)` and no default constructor, `BusTicket trip[4];` would fail to compile. Instead, the array is initialised via aggregate brace-initialization with explicit constructor invocations: `BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"), BusTicket("Chitra"), BusTicket("Dev")};`.

### Why `Token::next` Starts at 100 and Constructor Execution Count
In `token_predict.cpp`, `Token::next` starts at `100` so the first issued token gets `++next = 101` (three-digit token numbering), while `Token::issued()` computes `next - 100` to return the true count of tokens created (`4`). Constructing `Token counter[3]` invokes `Token()` `3` times (`101`, `102`, `103`), and `Token extra` invokes it a `4`th time (`104`), totalling **4 constructor executions**.

### Tickets Created Inside a Function Called Twice
In `bus_ticket_function_demo.cpp`, the local `BusTicket batch[2]` arrays have **automatic storage duration** and are constructed and destroyed on each return of `issueBatch`. However, `BusTicket::issued` has **static storage duration** for the lifetime of the program. Thus Call 1 leaves `issued == 2` (`Asha: Seat 1`, `Bala: Seat 2`), and Call 2 resumes from `2`, giving `Chitra: Seat 3` and placing `Dev` on `Waiting` (`issued == 3`).

## 5. Test Cases
- `token_predict.cpp`: `3`-element array + `1` extra token -> `101 103 104 4`.
- `complete_bus_ticket.cpp`: `4` passengers for capacity `3` -> `Seat 1`, `Seat 2`, `Seat 3`, `Waiting`, `Issued: 3`.
- `bus_ticket_function_demo.cpp`: `issueBatch` called twice (`2 + 2` passengers) -> confirms static persistence (`2 -> 3`) and waiting-list rollover.
- `bus_ticket_alternatives.cpp`: Two simultaneous buses (`BUS-101` cap `3` with `4` passengers, `BUS-202` cap `2` with `3` passengers) operate independently.

## 6. Predicted Output vs Actual Output
`token_predict.cpp` matched [`prediction.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/prediction.txt) (`101 103 104 4`), and `complete_bus_ticket.cpp` matched [`expected_output.txt`](file:///c:/Users/ANGULAKSHMI%20SUBBURAJ/DSA0115-192524402-Assignment2/Task05/expected_output.txt) verbatim.

## 7. Time and Space Complexity
- **Time complexity**: `O(1)` per ticket construction / seat allocation and `O(1)` for `count()` in all three approaches (global counter, static member, `TicketOffice`).
- **Space complexity**: `O(1)` shared counter storage (`1` `int` for the single-bus global or static member; `1` `TicketOffice` struct per bus when running multiple buses).

## 8. Alternative Approaches & Comparison
| Approach | Encapsulation | Multi-Bus Support | Time / Space |
| :--- | :--- | :--- | :--- |
| Global variable | Weak (any function can alter `g_issuedSeats`) | No (single shared global state) | `O(1)` / 1 counter |
| Static class member | Strong (private to `BusTicket`) | No (shared across entire `BusTicket` class) | `O(1)` / 1 counter |
| Separate `TicketOffice` object | Strong (private per-bus state) | Yes (one `TicketOffice` per bus) | `O(1)` / 1 counter per bus |

## 9. Design Decision and Reason
A `static` member (`BusTicket::issued`) neatly satisfies the single-bus requirement while keeping the counter private and accessible via `BusTicket::count()`. Whenever the college operates **two or more buses concurrently** with separate capacities, migrating to a `TicketOffice` object passed to each ticket removes class-wide coupling and supports independent buses cleanly.
