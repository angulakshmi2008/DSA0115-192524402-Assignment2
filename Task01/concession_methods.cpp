#include <iostream>

void applyByValue(long long fee, int percent) {
    if (fee < 0 || percent < 0 || percent > 100) {
        std::cout << "  [Value] Invalid fee or percentage.\n";
        return;
    }
    fee -= fee * percent / 100;
    std::cout << "  [Inside applyByValue] Revised local copy: " << fee << '\n';
}

void applyByReference(long long& fee, int percent) {
    if (fee < 0 || percent < 0 || percent > 100) {
        std::cout << "  [Reference] Invalid fee or percentage.\n";
        return;
    }
    fee -= fee * percent / 100;
}

void applyByPointer(long long* fee, int percent) {
    if (fee == nullptr) {
        std::cout << "  [Pointer] Null pointer passed; safely skipped.\n";
        return;
    }
    if (*fee < 0 || percent < 0 || percent > 100) {
        std::cout << "  [Pointer] Invalid fee or percentage.\n";
        return;
    }
    *fee -= *fee * percent / 100;
}

void testConcessionCase(const char* label, long long baseFee, int percent) {
    long long valueFee = baseFee;
    long long referenceFee = baseFee;
    long long pointerFee = baseFee;

    std::cout << "=== " << label << " (" << percent << "% on " << baseFee << ") ===\n";
    applyByValue(valueFee, percent);
    applyByReference(referenceFee, percent);
    applyByPointer(&pointerFee, percent);

    std::cout << "  Stored fee after call by value    : " << valueFee << '\n';
    std::cout << "  Stored fee after call by reference: " << referenceFee << '\n';
    std::cout << "  Stored fee after call by pointer  : " << pointerFee << "\n\n";
}

int main() {
    const long long baseFee = 45000;

    // PDF concession examples: merit 10%, sports 15%, single parent 25%
    testConcessionCase("Merit Concession", baseFee, 10);
    testConcessionCase("Sports Concession", baseFee, 15);
    testConcessionCase("Single Parent Concession", baseFee, 25);

    // Defensive null pointer and validation edge cases
    std::cout << "=== Edge Cases ===\n";
    applyByPointer(nullptr, 10);

    long long zeroPercentFee = baseFee;
    applyByReference(zeroPercentFee, 0);
    std::cout << "  Stored fee after 0% concession    : " << zeroPercentFee << '\n';

    long long invalidFee = baseFee;
    applyByReference(invalidFee, 120);
    std::cout << "  Stored fee after invalid 120%     : " << invalidFee << "\n\n";

    // Sequential concessions on one student
    std::cout << "=== Sequential Concessions (Merit 10% then Sports 15%) ===\n";
    long long studentFee = baseFee;
    std::cout << "  Initial hostel fee                : " << studentFee << '\n';
    applyByReference(studentFee, 10);
    std::cout << "  After Merit 10%                   : " << studentFee << '\n';
    applyByReference(studentFee, 15);
    std::cout << "  After Sports 15%                  : " << studentFee << '\n';

    return 0;
}
