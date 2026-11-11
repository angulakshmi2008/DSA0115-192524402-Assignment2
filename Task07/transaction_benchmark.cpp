#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
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
};

namespace {
using Clock = std::chrono::steady_clock;

long long checksumOf(const std::vector<long long>& items) {
    long long sum = 0;
    for (std::size_t i = 0; i < items.size(); ++i) {
        sum += items[i] * static_cast<long long>(i + 1);
    }
    return sum;
}

template <typename HistoryT>
double measureInsertions(int nAdditions, long long seedOffset, long long& outChecksum) {
    HistoryT history;
    auto start = Clock::now();
    for (int i = 1; i <= nAdditions; ++i) {
        history.add(static_cast<long long>(i) + seedOffset);
    }
    auto stop = Clock::now();

    auto snap = history.snapshot();
    volatile long long sink = checksumOf(snap);
    outChecksum = sink;

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    return static_cast<double>(ns) / 1e6;
}

template <int CAPACITY>
void runSuite(const char* title, long long seedOffset) {
    const int workloads[3] = {100000, 1000000, 5000000};
    const int trials = 3;

    std::cout << "=== " << title << " (CAPACITY = " << CAPACITY << ", " << trials
              << " trials each) ===\n";

    for (int n : workloads) {
        std::cout << "\nWorkload: " << n << " insertions\n";
        for (int t = 1; t <= trials; ++t) {
            long long csOrig = 0;
            long long csCirc = 0;
            double msOrig = measureInsertions<OriginalShiftingHistory<CAPACITY>>(n, seedOffset, csOrig);
            double msCirc = measureInsertions<RedesignedCircularHistory<CAPACITY>>(n, seedOffset, csCirc);

            if (csOrig != csCirc) {
                std::cerr << "ERROR: Snapshot checksum mismatch at n=" << n << "!\n";
                std::exit(1);
            }

            std::cout << "  Trial " << t << " -> Shifting: " << std::setw(8) << std::fixed
                      << std::setprecision(3) << msOrig << " ms | Circular: "
                      << std::setw(8) << msCirc << " ms | Checksum: " << csOrig << '\n';
        }
    }
    std::cout << '\n';
}
} // namespace

int main(int argc, char* argv[]) {
    long long seedOffset = (argc > 0) ? static_cast<long long>(argc) : 1LL;
    (void)argv;

    runSuite<10>("Primary Redesign Comparison", seedOffset);
    runSuite<50>("Expanded Requirement Comparison", seedOffset);
    return 0;
}
