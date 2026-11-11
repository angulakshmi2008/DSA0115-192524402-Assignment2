#include <iomanip>
#include <iostream>
#include <type_traits>

// Alternative 1: Single ParcelOptions struct with default member initializers.
// Adding a new option (such as 'fragile') extends the struct centrally without
// multiplying overload combinations.
struct ParcelOptions {
    double weightKg = 0.0;
    bool express = false;
    bool insured = false;
    bool fragile = false;
};

double chargeStruct(const ParcelOptions& p) {
    if (p.weightKg <= 0.0) {
        return 0.0;
    }
    double amount = p.weightKg * 40.0;
    if (p.express) amount += 30.0;
    if (p.insured) amount += 20.0;
    if (p.fragile) amount += 15.0;
    return amount;
}

// Alternative 2: Function template handling integral (grams) and floating-point (kg)
// weights with C++17 compile-time dispatch.
template <typename WeightT>
double chargeTemplate(WeightT weight, bool express = false, bool insured = false) {
    static_assert(std::is_arithmetic_v<WeightT> && !std::is_same_v<WeightT, bool>,
                  "WeightT must be a numeric type (not bool).");
    double weightKg = 0.0;
    if constexpr (std::is_integral_v<WeightT>) {
        weightKg = static_cast<double>(weight) / 1000.0;
    } else {
        weightKg = static_cast<double>(weight);
    }
    if (weightKg <= 0.0) {
        return 0.0;
    }
    double amount = weightKg * 40.0;
    if (express) amount += 30.0;
    if (insured) amount += 20.0;
    return amount;
}

int main() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Alternative 1: Single ParcelOptions Struct ===\n";
    ParcelOptions standard{2.0};
    ParcelOptions fragileExpress{2.5, true, true, true};
    std::cout << "Standard (2.0 kg)                 : Rs. " << chargeStruct(standard) << '\n';
    std::cout << "Express + Insured + Fragile (2.5) : Rs. " << chargeStruct(fragileExpress) << '\n';

    std::cout << "\n=== Alternative 2: Function Template ===\n";
    std::cout << "chargeTemplate(500) [int grams]   : Rs. " << chargeTemplate(500) << '\n';
    std::cout << "chargeTemplate(2.0f, true) [float]: Rs. " << chargeTemplate(2.0f, true) << '\n';
    std::cout << "chargeTemplate(2.0, false, true)  : Rs. " << chargeTemplate(2.0, false, true) << '\n';

    std::cout << "\n=== Parameter Copy Cost Comparison ===\n";
    std::cout << "sizeof(double)               : " << sizeof(double) << " bytes\n";
    std::cout << "sizeof(ParcelOptions)        : " << sizeof(ParcelOptions) << " bytes\n";
    std::cout << "sizeof(const ParcelOptions*) : " << sizeof(const ParcelOptions*) << " bytes (const ref binding)\n";

    return 0;
}
