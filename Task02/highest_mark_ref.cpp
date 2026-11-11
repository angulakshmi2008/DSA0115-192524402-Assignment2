#include <iostream>
#include <stdexcept>
#include <vector>

int& highestMark(int marks[], int size) {
    if (marks == nullptr || size <= 0) {
        throw std::invalid_argument("Marks array must be non-empty.");
    }
    int highestIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[highestIndex]) {
            highestIndex = i;
        }
    }
    return marks[highestIndex];
}

const int& readHighestMark(const int marks[], int size) {
    if (marks == nullptr || size <= 0) {
        throw std::invalid_argument("Marks array must be non-empty.");
    }
    int highestIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[highestIndex]) {
            highestIndex = i;
        }
    }
    return marks[highestIndex];
}

int copyHighestMark(const int marks[], int size) {
    return readHighestMark(marks, size);
}

int& highestMarkVec(std::vector<int>& marks) {
    if (marks.empty()) {
        throw std::invalid_argument("Marks vector must be non-empty.");
    }
    std::size_t highestIndex = 0;
    for (std::size_t i = 1; i < marks.size(); ++i) {
        if (marks[i] > marks[highestIndex]) {
            highestIndex = i;
        }
    }
    return marks[highestIndex];
}

void printMarks(const char* label, const int marks[], int size) {
    std::cout << label << ": [";
    for (int i = 0; i < size; ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << marks[i];
    }
    std::cout << "]\n";
}

int main() {
    // 1. Direct +2 moderation using returned reference
    int marks[] = {78, 91, 84, 88, 86};
    const int size = static_cast<int>(sizeof(marks) / sizeof(marks[0]));

    std::cout << "=== 1. Direct Moderation (+2) via Return by Reference ===\n";
    printMarks("Initial marks", marks, size);
    std::cout << "Highest before moderation: " << highestMark(marks, size) << '\n';
    highestMark(marks, size) += 2;
    std::cout << "Highest after +2         : " << highestMark(marks, size) << '\n';
    printMarks("Marks after +2", marks, size);

    // 2. Using the function call on the left side of '='
    std::cout << "\n=== 2. Function Call on Left of '=' ===\n";
    int examMarks[] = {72, 96, 85, 91};
    const int examSize = static_cast<int>(sizeof(examMarks) / sizeof(examMarks[0]));
    printMarks("Before direct assignment", examMarks, examSize);
    highestMark(examMarks, examSize) = 100;
    printMarks("After highestMark(...)=100", examMarks, examSize);

    // 3. Comparing int&, const int&, and int copy
    std::cout << "\n=== 3. Reference vs Const Reference vs Copy ===\n";
    const int& roRef = readHighestMark(marks, size);
    int localCopy = copyHighestMark(marks, size);
    localCopy += 5; // Modifies only the independent local copy
    std::cout << "const int& readHighestMark : " << roRef << '\n';
    std::cout << "Modified local int copy    : " << localCopy << '\n';
    std::cout << "Actual marks[1] unchanged  : " << marks[1] << '\n';

    // 4. Changed Condition: std::vector growth and buffer reallocation (observed safely)
    std::cout << "\n=== 4. std::vector Reallocation Invalidates References ===\n";
    std::vector<int> vecMarks{78, 91, 84};
    int& vecRef = highestMarkVec(vecMarks);
    const int* oldElementAddr = &vecRef;
    const std::size_t oldCapacity = vecMarks.capacity();
    std::cout << "Before push_back: size=" << vecMarks.size()
              << ", capacity=" << oldCapacity
              << ", &highest=" << static_cast<const void*>(oldElementAddr)
              << ", value=" << vecRef << '\n';

    // Force reallocation without dereferencing oldElementAddr/vecRef afterwards
    for (int extra = 80; vecMarks.capacity() == oldCapacity; ++extra) {
        vecMarks.push_back(extra);
    }

    const int* newHighestAddr = &highestMarkVec(vecMarks);
    std::cout << "After growth    : size=" << vecMarks.size()
              << ", capacity=" << vecMarks.capacity()
              << ", new &highest=" << static_cast<const void*>(newHighestAddr) << '\n';
    std::cout << "Address changed (old reference invalidated): "
              << (oldElementAddr != newHighestAddr ? "YES" : "NO") << '\n';

    return 0;
}
