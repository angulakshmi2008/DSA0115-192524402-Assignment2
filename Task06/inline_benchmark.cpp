#include "time_outofline.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

// Same-TU non-inline functions (compiler in the same TU may still inline at -O2)
int minutesBetweenSameTU(const TimeBench& a, const TimeBench& b) {
    return minutesBetweenInline(a, b);
}

bool earlierSameTU(const TimeBench& a, const TimeBench& b) {
    return earlierInline(a, b);
}

namespace {
using Clock = std::chrono::steady_clock;

template <typename DiffFn, typename CmpFn>
double runBenchmark(const char* label,
                    const std::vector<TimeBench>& schedule,
                    int iterations,
                    DiffFn diffFn,
                    CmpFn cmpFn,
                    long long& outChecksum) {
    long long checksum = 0;
    const std::size_t mask = schedule.size() - 1;

    auto start = Clock::now();
    for (int i = 0; i < iterations; ++i) {
        const TimeBench& a = schedule[static_cast<std::size_t>(i) & mask];
        const TimeBench& b = schedule[static_cast<std::size_t>(i + 17) & mask];
        checksum += diffFn(a, b);
        if (cmpFn(a, b)) {
            ++checksum;
        }
    }
    auto stop = Clock::now();

    volatile long long sink = checksum;
    outChecksum = sink;

    auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    double elapsedMs = static_cast<double>(elapsedNs) / 1e6;
    std::cout << "  " << std::left << std::setw(32) << label << ": "
              << std::right << std::setw(10) << std::fixed << std::setprecision(3)
              << elapsedMs << " ms (checksum=" << outChecksum << ")\n";
    return elapsedMs;
}
} // namespace

int main(int argc, char* argv[]) {
    const int iterations = 10000000;
    const int runs = 3;

    // 64 Time entries initialized from runtime seed (argc) so optimizer cannot
    // precompute the table, while workload remains identical across all variants.
    std::vector<TimeBench> schedule;
    schedule.reserve(64);
    int offset = (argc > 0) ? (argc % 3) : 1;
    for (int i = 0; i < 64; ++i) {
        int h = (i * 7 + offset) % 24;
        int m = (i * 13 + offset * 5) % 60;
        schedule.emplace_back(h, m);
    }

    std::cout << "Benchmark: " << iterations << " calls per function pair ("
              << runs << " trials each)\n\n";

    for (int r = 1; r <= runs; ++r) {
        std::cout << "--- Run " << r << " ---\n";
        long long cs1 = 0, cs2 = 0, cs3 = 0, cs4 = 0, cs5 = 0;

        runBenchmark("1. Separate-TU (no inline)", schedule, iterations,
                     [](const TimeBench& a, const TimeBench& b) { return minutesBetweenOutOfLine(a, b); },
                     [](const TimeBench& a, const TimeBench& b) { return earlierOutOfLine(a, b); },
                     cs1);
        runBenchmark("2. Same-TU (no inline keyword)", schedule, iterations,
                     [](const TimeBench& a, const TimeBench& b) { return minutesBetweenSameTU(a, b); },
                     [](const TimeBench& a, const TimeBench& b) { return earlierSameTU(a, b); },
                     cs2);
        runBenchmark("3. Inline friend functions", schedule, iterations,
                     [](const TimeBench& a, const TimeBench& b) { return minutesBetweenInline(a, b); },
                     [](const TimeBench& a, const TimeBench& b) { return earlierInline(a, b); },
                     cs3);
        runBenchmark("4. Inline member functions", schedule, iterations,
                     [](const TimeBench& a, const TimeBench& b) { return a.minutesToMember(b); },
                     [](const TimeBench& a, const TimeBench& b) { return a.isEarlierMember(b); },
                     cs4);
        runBenchmark("5. Inline getter functions", schedule, iterations,
                     [](const TimeBench& a, const TimeBench& b) { return minutesBetweenGetters(a, b); },
                     [](const TimeBench& a, const TimeBench& b) { return earlierGetters(a, b); },
                     cs5);

        if (cs1 != cs2 || cs2 != cs3 || cs3 != cs4 || cs4 != cs5) {
            std::cerr << "ERROR: Checksum mismatch across variants!\n";
            return 1;
        }
        std::cout << '\n';
    }

    (void)argv;
    return 0;
}
