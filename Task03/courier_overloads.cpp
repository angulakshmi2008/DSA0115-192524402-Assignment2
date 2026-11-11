#include <iomanip>
#include <iostream>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

namespace {
// Illustrative courier rates (assignment PDF does not prescribe actual prices)
constexpr double RATE_PER_KG = 40.0;
constexpr double EXPRESS_FEE = 30.0;
constexpr double INSURANCE_FEE = 20.0;

double computeAmount(double weightKg, bool express, bool insured) {
    if (weightKg <= 0.0) {
        return 0.0;
    }
    double amount = weightKg * RATE_PER_KG;
    if (express) {
        amount += EXPRESS_FEE;
    }
    if (insured) {
        amount += INSURANCE_FEE;
    }
    return amount;
}
} // namespace

double charge(double weightKg, bool express = false, bool insured = false) {
    std::cout << "  [Selected: charge(double, bool, bool)] ";
    return computeAmount(weightKg, express, insured);
}

double charge(int weightGrams, bool express = false, bool insured = false) {
    std::cout << "  [Selected: charge(int, bool, bool)]    ";
    return computeAmount(weightGrams / 1000.0, express, insured);
}

double charge(const Parcel& parcel) {
    std::cout << "  [Selected: charge(const Parcel&)]      ";
    return computeAmount(parcel.weightKg, parcel.express, parcel.insured);
}

int main() {
    Parcel box{2.5, true, false};

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Note: Illustrative courier rates (Rs. 40/kg, +30 express, +20 insured).\n\n";

    std::cout << "=== Six Designed Overload Calls ===\n";
    double c1 = charge(2);
    std::cout << "1. charge(2)                -> Rs. " << c1 << '\n';
    double c2 = charge(2.0f);
    std::cout << "2. charge(2.0f)             -> Rs. " << c2 << '\n';
    double c3 = charge(500);
    std::cout << "3. charge(500)              -> Rs. " << c3 << '\n';
    double c4 = charge(2, true);
    std::cout << "4. charge(2, true)          -> Rs. " << c4 << '\n';
    double c5 = charge(2.0, false, true);
    std::cout << "5. charge(2.0, false, true) -> Rs. " << c5 << '\n';
    double c6 = charge(box);
    std::cout << "6. charge(box)              -> Rs. " << c6 << '\n';

    std::cout << "\n=== Additional Promotion & Boundary Cases ===\n";
    short smallGrams = 750;
    double e1 = charge(smallGrams);
    std::cout << "short(750) promoted to int  -> Rs. " << e1 << '\n';
    double e2 = charge(2.0);
    std::cout << "charge(2.0) exact double    -> Rs. " << e2 << '\n';
    double e3 = charge(-5.0);
    std::cout << "charge(-5.0) invalid        -> Rs. " << e3 << '\n';
    double e4 = charge(0);
    std::cout << "charge(0) zero weight       -> Rs. " << e4 << '\n';

    return 0;
}
