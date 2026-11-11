#include <deque>
#include <iostream>
#include <vector>

class ShiftingHistory {
    static const int CAPACITY = 10;
    long long data[CAPACITY]{};
    int count = 0;

public:
    void add(long long amount) {
        if (count < CAPACITY) {
            data[count++] = amount;
        } else {
            for (int i = 1; i < CAPACITY; ++i) {
                data[i - 1] = data[i];
            }
            data[CAPACITY - 1] = amount;
        }
    }

    std::vector<long long> snapshot() const {
        return std::vector<long long>(data, data + count);
    }
};

class CircularHistory {
    static const int CAPACITY = 10;
    long long data[CAPACITY]{};
    int start = 0;
    int count = 0;

public:
    void add(long long amount) {
        int pos = (start + count) % CAPACITY;
        data[pos] = amount;
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
            out.push_back(data[(start + i) % CAPACITY]);
        }
        return out;
    }
};

class DequeHistory {
    static const std::size_t CAPACITY = 10;
    std::deque<long long> data;

public:
    void add(long long amount) {
        if (data.size() == CAPACITY) {
            data.pop_front();
        }
        data.push_back(amount);
    }

    std::vector<long long> snapshot() const {
        return std::vector<long long>(data.begin(), data.end());
    }
};

void printHistory(const char* label, const std::vector<long long>& items) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << (items[i] >= 0 ? "+" : "") << items[i];
    }
    std::cout << "]\n";
}

int main() {
    const long long sequence[12] = {
        5000, -2000, 1500, 3000, -1200, 2500,
        -800, 4000, -1500, 1000, -500, 6000
    };

    ShiftingHistory shifting;
    CircularHistory circular;
    DequeHistory dequeHist;

    for (long long tx : sequence) {
        shifting.add(tx);
        circular.add(tx);
        dequeHist.add(tx);
    }

    auto s1 = shifting.snapshot();
    auto s2 = circular.snapshot();
    auto s3 = dequeHist.snapshot();

    std::cout << "=== Comparison After 12 Transactions (Capacity 10) ===\n";
    printHistory("1. Shifting Fixed Array", s1);
    printHistory("2. Circular Fixed Array", s2);
    printHistory("3. std::deque History  ", s3);
    std::cout << "All three outputs identical: "
              << ((s1 == s2 && s2 == s3) ? "YES" : "NO") << '\n';

    return 0;
}
