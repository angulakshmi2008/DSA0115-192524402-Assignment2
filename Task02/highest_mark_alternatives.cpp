#include <iostream>
#include <vector>

int highestIndex(const int marks[], int size) {
    if (marks == nullptr || size <= 0) {
        return -1;
    }
    int best = 0;
    for (int i = 1; i < size; ++i) {
        if (marks[i] > marks[best]) {
            best = i;
        }
    }
    return best;
}

int* highestPointer(int marks[], int size) {
    if (marks == nullptr || size <= 0) {
        return nullptr;
    }
    int* best = &marks[0];
    for (int i = 1; i < size; ++i) {
        if (marks[i] > *best) {
            best = &marks[i];
        }
    }
    return best;
}

int highestIndexVec(const std::vector<int>& marks) {
    if (marks.empty()) {
        return -1;
    }
    int best = 0;
    for (int i = 1; i < static_cast<int>(marks.size()); ++i) {
        if (marks[i] > marks[best]) {
            best = i;
        }
    }
    return best;
}

int main() {
    // 1. Index alternative
    int marksA[] = {78, 91, 84, 88, 86};
    const int sizeA = static_cast<int>(sizeof(marksA) / sizeof(marksA[0]));
    int idx = highestIndex(marksA, sizeA);
    std::cout << "=== 1. Alternative: Return Index ===\n";
    if (idx != -1) {
        std::cout << "Highest index: " << idx << ", value: " << marksA[idx] << '\n';
        marksA[idx] += 2;
        std::cout << "After +2     : " << marksA[idx] << '\n';
    }

    // 2. Pointer alternative
    int marksB[] = {78, 91, 84, 88, 86};
    const int sizeB = static_cast<int>(sizeof(marksB) / sizeof(marksB[0]));
    int* ptr = highestPointer(marksB, sizeB);
    std::cout << "\n=== 2. Alternative: Return Pointer ===\n";
    if (ptr != nullptr) {
        std::cout << "Highest via pointer: " << *ptr << '\n';
        *ptr += 2;
        std::cout << "After +2           : " << *ptr << '\n';
    }

    // 3. Edge cases: ties, single element, empty array
    std::cout << "\n=== 3. Edge Cases ===\n";
    int tiedMarks[] = {80, 95, 95, 70};
    std::cout << "Tie first highest index : " << highestIndex(tiedMarks, 4)
              << " (value " << tiedMarks[highestIndex(tiedMarks, 4)] << ")\n";

    int singleMark[] = {64};
    int* singlePtr = highestPointer(singleMark, 1);
    *singlePtr = 66;
    std::cout << "Single element updated  : " << singleMark[0] << '\n';

    std::cout << "Empty array index       : " << highestIndex(nullptr, 0) << '\n';
    std::cout << "Empty array pointer     : "
              << (highestPointer(nullptr, 0) == nullptr ? "nullptr" : "non-null") << '\n';

    // 4. Index safety across vector reallocation
    std::cout << "\n=== 4. Index Stability After Vector Reallocation ===\n";
    std::vector<int> vec{75, 92, 81};
    int savedIdx = highestIndexVec(vec);
    std::size_t capBefore = vec.capacity();
    while (vec.capacity() == capBefore) {
        vec.push_back(70);
    }
    std::cout << "Capacity grew from " << capBefore << " to " << vec.capacity() << '\n';
    std::cout << "Element at saved index " << savedIdx << " after reallocation: "
              << vec[savedIdx] << '\n';

    return 0;
}
