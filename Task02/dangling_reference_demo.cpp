#include <iostream>

// Deliberately unsafe function kept isolated solely to inspect compiler diagnostics
// (-Wreturn-local-addr). Do NOT call and dereference the returned reference.
int& unsafeReference() {
    int localValue = 50;
    return localValue; // Dangling reference: localValue lifetime ends on return
}

int main() {
    std::cout << "unsafeReference() is compiled only to demonstrate the compiler warning.\n";
    std::cout << "Its returned reference is NOT dereferenced because doing so is undefined behaviour.\n";
    return 0;
}
