#include <iostream>
#include <string>
#include <vector>

template <int CAPACITY>
class OriginalShiftingHistory {
    long long values[CAPACITY]{};
    int count = 0;

public:
    void add(long long amount) {
        if (count < CAPACITY) {
            values[count++] = amount;
        } else {
            for (int i = 1; i < CAPACITY; ++i) {
                values[i - 1] = values[i];
            }
            values[CAPACITY - 1] = amount;
        }
    }

    std::vector<long long> snapshot() const {
        return std::vector<long long>(values, values + count);
    }

    void print() const {
        for (int i = 0; i < count; ++i) {
            if (i > 0) std::cout << ' ';
            std::cout << values[i];
        }
        std::cout << '\n';
    }
};

template <int CAPACITY>
class RedesignedCircularHistory {
    long long values[CAPACITY]{};
    int start = 0;
    int count = 0;

public:
    void add(long long amount) {
        int position = (start + count) % CAPACITY;
        values[position] = amount;
        if (count < CAPACITY) {
            ++count;
        } else {
            start = (start + 1) % CAPACITY;
        }
    }

    std::vector<long long> snapshot() const {
        std::vector<long long> out;
        out.reserve(count);
        for (int i = 0; i < count; ++i) {
            out.push_back(values[(start + i) % CAPACITY]);
        }
        return out;
    }

    void print() const {
        for (int i = 0; i < count; ++i) {
            if (i > 0) std::cout << ' ';
            std::cout << values[(start + i) % CAPACITY];
        }
        std::cout << '\n';
    }
};

template <typename HistoryT>
class SavingsAccountModel {
    std::string accountNumber;
    long long balancePaise;
    HistoryT history;

public:
    SavingsAccountModel(const std::string& number, long long opening)
        : accountNumber(number), balancePaise(opening >= 0 ? opening : 0) {}

    bool deposit(long long amount) {
        if (amount <= 0) return false;
        balancePaise += amount;
        history.add(amount);
        return true;
    }

    bool withdraw(long long amount) {
        if (amount <= 0 || amount > balancePaise) return false;
        balancePaise -= amount;
        history.add(-amount);
        return true;
    }

    long long balance() const { return balancePaise; }
    std::vector<long long> statement() const { return history.snapshot(); }
    void printStatement(const char* label) const {
        std::cout << label << " (" << accountNumber << ", Balance=" << balancePaise << "): ";
        history.print();
    }
};

int main() {
    std::cout << "=== 1. 12-Transaction Sequence (100..1200, Capacity 10) ===\n";
    OriginalShiftingHistory<10> origSeq;
    RedesignedCircularHistory<10> circSeq;
    for (int i = 1; i <= 12; ++i) {
        origSeq.add(i * 100);
        circSeq.add(i * 100);
    }
    std::cout << "Original Shifting  : ";
    origSeq.print();
    std::cout << "Redesigned Circular: ";
    circSeq.print();
    std::cout << "Identical output   : "
              << (origSeq.snapshot() == circSeq.snapshot() ? "PASS" : "FAIL") << "\n\n";

    std::cout << "=== 2. SavingsAccount Sequence (Same Inputs as Task 04) ===\n";
    SavingsAccountModel<OriginalShiftingHistory<10>> origAcc("SA1001", 100000);
    SavingsAccountModel<RedesignedCircularHistory<10>> circAcc("SA1001", 100000);

    struct Op { char kind; long long amt; };
    const Op ops[] = {
        {'D', 5000}, {'W', 2000}, {'D', 1500}, {'D', 0}, {'W', -100},
        {'W', 500000}, {'D', 3000}, {'W', 1200}, {'D', 2500}, {'W', 800},
        {'D', 4000}, {'W', 1500}, {'D', 1000}, {'W', 500}, {'D', 6000}
    };

    for (const Op& op : ops) {
        if (op.kind == 'D') {
            origAcc.deposit(op.amt);
            circAcc.deposit(op.amt);
        } else {
            origAcc.withdraw(op.amt);
            circAcc.withdraw(op.amt);
        }
    }

    origAcc.printStatement("Original Account  ");
    circAcc.printStatement("Redesigned Account");
    bool accMatch = (origAcc.balance() == circAcc.balance()) &&
                    (origAcc.statement() == circAcc.statement());
    std::cout << "Account equivalence: " << (accMatch ? "PASS" : "FAIL") << "\n\n";

    std::cout << "=== 3. Boundary Fill-Level Checks (0, 1, 9, 10, 11, 25 items) ===\n";
    const int counts[] = {0, 1, 9, 10, 11, 25};
    bool allPassed = true;
    for (int n : counts) {
        OriginalShiftingHistory<10> a;
        RedesignedCircularHistory<10> b;
        for (int i = 1; i <= n; ++i) {
            a.add(i * 10);
            b.add(i * 10);
        }
        bool ok = (a.snapshot() == b.snapshot());
        allPassed = allPassed && ok;
        std::cout << "  n=" << n << " -> " << (ok ? "MATCH" : "MISMATCH") << '\n';
    }
    std::cout << "Overall validation : " << (allPassed && accMatch ? "ALL PASS" : "FAILED") << '\n';

    return (allPassed && accMatch) ? 0 : 1;
}
