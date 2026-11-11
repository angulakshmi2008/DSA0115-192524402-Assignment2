#include <iomanip>
#include <iostream>

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    int hour() const { return h; }
    int minute() const { return m; }
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

int minutesBetween(const Time& a, const Time& b) {
    int first = a.h * 60 + a.m;
    int second = b.h * 60 + b.m;
    int diff = first - second;
    return diff < 0 ? -diff : diff;
}

bool earlier(const Time& a, const Time& b) {
    if (a.h != b.h) {
        return a.h < b.h;
    }
    return a.m < b.m;
}

class ValidatedTime {
    int h, m;
    bool valid;

public:
    ValidatedTime(int hh, int mm) : h(0), m(0), valid(false) {
        if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) {
            h = hh;
            m = mm;
            valid = true;
        }
    }

    bool isValid() const { return valid; }
    int hour() const { return h; }
    int minute() const { return m; }
};

void runCase(const char* label, const Time& a, const Time& b) {
    std::cout << label << " ("
              << std::setfill('0') << std::setw(2) << a.hour() << ':'
              << std::setw(2) << a.minute() << " vs "
              << std::setw(2) << b.hour() << ':'
              << std::setw(2) << b.minute() << "): "
              << "minutesBetween=" << minutesBetween(a, b)
              << ", earlier(a,b)=" << std::boolalpha << earlier(a, b)
              << ", earlier(b,a)=" << earlier(b, a) << '\n';
}

int main() {
    std::cout << "=== Time Correctness Tests ===\n";
    runCase("PDF sample       ", Time(9, 45), Time(13, 10));
    runCase("Same time twice  ", Time(10, 30), Time(10, 30));
    runCase("Across noon      ", Time(11, 50), Time(12, 10));
    runCase("Later dep/early  ", Time(14, 15), Time(13, 45));
    runCase("Day boundaries   ", Time(0, 0), Time(23, 59));

    std::cout << "\n=== Hour/Minute Boundary Validation ===\n";
    ValidatedTime okMin(0, 0);
    ValidatedTime okMax(23, 59);
    ValidatedTime badHour(24, 0);
    ValidatedTime badMinute(12, 60);
    ValidatedTime badNeg(-1, 15);
    std::cout << "00:00 valid : " << std::boolalpha << okMin.isValid() << '\n';
    std::cout << "23:59 valid : " << okMax.isValid() << '\n';
    std::cout << "24:00 valid : " << badHour.isValid() << '\n';
    std::cout << "12:60 valid : " << badMinute.isValid() << '\n';
    std::cout << "-1:15 valid : " << badNeg.isValid() << '\n';

    return 0;
}
