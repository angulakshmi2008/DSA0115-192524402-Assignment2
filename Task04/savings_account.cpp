#include <fstream>
#include <iostream>
#include <string>

class SavingsAccount {
private:
    static const int CAPACITY = 10;
    std::string accountNumber;
    long long balancePaise;
    long long transactions[CAPACITY];
    int count;

    // Defined inside the class
    void record(long long amount) {
        if (count < CAPACITY) {
            transactions[count++] = amount;
        } else {
            for (int i = 1; i < CAPACITY; ++i) {
                transactions[i - 1] = transactions[i];
            }
            transactions[CAPACITY - 1] = amount;
        }
    }

public:
    // Defined inside the class
    explicit SavingsAccount(const std::string& number, long long openingBalance)
        : accountNumber(number),
          balancePaise(openingBalance >= 0 ? openingBalance : 0),
          transactions{},
          count(0) {}

    long long getBalance() const { return balancePaise; }
    int getTransactionCount() const { return count; }

    // Declared inside, defined outside using scope resolution operator ::
    bool deposit(long long amount);
    bool withdraw(long long amount);
    void miniStatement() const;
};

bool SavingsAccount::deposit(long long amount) {
    if (amount <= 0) {
        return false;
    }
    balancePaise += amount;
    record(amount);
    return true;
}

bool SavingsAccount::withdraw(long long amount) {
    if (amount <= 0 || amount > balancePaise) {
        return false;
    }
    balancePaise -= amount;
    record(-amount);
    return true;
}

void SavingsAccount::miniStatement() const {
    std::cout << "Account: " << accountNumber
              << " | Stored transactions: " << count
              << " | Balance (paise): " << balancePaise << '\n';
    if (count == 0) {
        std::cout << "  (No transactions recorded)\n";
        return;
    }
    for (int i = 0; i < count; ++i) {
        std::cout << "  [" << (i + 1) << "] "
                  << (transactions[i] >= 0 ? "+" : "") << transactions[i] << '\n';
    }
}

namespace {
std::ifstream openInputFile(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream custom(argv[1]);
        if (custom.is_open()) return custom;
    }
    std::ifstream local("transactions_input.txt");
    if (local.is_open()) return local;
    return std::ifstream("Task04/transactions_input.txt");
}
} // namespace

int main(int argc, char* argv[]) {
    std::ifstream in = openInputFile(argc, argv);
    if (!in.is_open()) {
        std::cerr << "Failed to open transactions_input.txt\n";
        return 1;
    }

    std::string accNo;
    long long opening = 0;
    in >> accNo >> opening;

    SavingsAccount account(accNo, opening);
    std::cout << "=== Initial State ===\n";
    account.miniStatement();

    char op = '\0';
    long long amount = 0;
    int validCount = 0;

    std::cout << "\n=== Processing Transactions ===\n";
    while (in >> op >> amount) {
        if (op == 'D') {
            if (account.deposit(amount)) {
                ++validCount;
                std::cout << "Valid #" << validCount << " Deposit  +" << amount
                          << " -> Balance: " << account.getBalance() << '\n';
            } else {
                std::cout << "Rejected Deposit   " << amount << " (invalid amount)\n";
            }
        } else if (op == 'W') {
            if (account.withdraw(amount)) {
                ++validCount;
                std::cout << "Valid #" << validCount << " Withdraw -" << amount
                          << " -> Balance: " << account.getBalance() << '\n';
            } else {
                std::cout << "Rejected Withdraw  " << amount
                          << " (invalid or insufficient funds)\n";
            }
        }

        if (validCount == 10 || validCount == 11 || validCount == 12) {
            std::cout << "\n--- Mini-Statement after valid transaction #"
                      << validCount << " ---\n";
            account.miniStatement();
            std::cout << '\n';
        }
    }

    return 0;
}
