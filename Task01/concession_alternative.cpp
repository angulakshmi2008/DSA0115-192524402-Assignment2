#include <iostream>

struct Student {
    int regNo;
    char name[64];
    char department[48];
    long long hostelFee;
    double cgpa;
    int semesterMarks[8];
};

long long applyConcessionReturn(long long fee, int percent) {
    if (fee < 0 || percent < 0 || percent > 100) {
        return fee;
    }
    return fee - fee * percent / 100;
}

void printStudentByValue(Student s) {
    std::cout << "  [By value] Student: " << s.name
              << ", RegNo: " << s.regNo
              << ", Fee: " << s.hostelFee << '\n';
}

void printStudentByConstRef(const Student& s) {
    std::cout << "  [By const ref] Student: " << s.name
              << ", RegNo: " << s.regNo
              << ", Fee: " << s.hostelFee << '\n';
}

void applyStudentConcessionByRef(Student& s, int percent) {
    s.hostelFee = applyConcessionReturn(s.hostelFee, percent);
}

int main() {
    const long long baseFee = 45000;

    // Return-by-value keeps original fee intact while supporting chaining
    long long afterMerit = applyConcessionReturn(baseFee, 10);
    long long afterMeritAndSports = applyConcessionReturn(afterMerit, 15);
    long long afterAllThree = applyConcessionReturn(afterMeritAndSports, 25);

    std::cout << "=== Alternative: Return New Fee ===\n";
    std::cout << "  Original baseFee        : " << baseFee << '\n';
    std::cout << "  After Merit (10%)       : " << afterMerit << '\n';
    std::cout << "  After +Sports (15%)     : " << afterMeritAndSports << '\n';
    std::cout << "  After +SingleParent(25%): " << afterAllThree << "\n\n";

    Student student{192524402, "ANGULAKSHMI S.", "Information Technology", baseFee, 9.1,
                    {88, 91, 86, 90, 89, 92, 87, 93}};

    std::cout << "=== Parameter Size Comparison ===\n";
    std::cout << "  sizeof(long long)   : " << sizeof(long long) << " bytes\n";
    std::cout << "  sizeof(Student)     : " << sizeof(Student) << " bytes\n";
    std::cout << "  sizeof(Student*)    : " << sizeof(Student*) << " bytes (pointer/ref representation)\n\n";

    std::cout << "=== Passing Student Object ===\n";
    printStudentByValue(student);
    printStudentByConstRef(student);
    applyStudentConcessionByRef(student, 25);
    std::cout << "  After 25% by Student&   : " << student.hostelFee << '\n';

    return 0;
}
