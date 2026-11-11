// Intentionally broken original overload design from Q3 problem statement.
// Preserved separately only to capture the genuine compiler ambiguity error.

double charge(double weight) {
    return weight * 40.0;
}

double charge(double weight, bool express) {
    return weight * 40.0 + (express ? 30.0 : 0.0);
}

double charge(double weight, bool express = false, bool insured = false) {
    return weight * 40.0 + (express ? 30.0 : 0.0) + (insured ? 20.0 : 0.0);
}

int main() {
    double a = charge(2.0);
    double b = charge(2.0, true);
    return (a + b > 0.0) ? 0 : 1;
}
