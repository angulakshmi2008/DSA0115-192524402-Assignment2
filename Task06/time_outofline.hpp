#ifndef TASK06_TIME_OUTOFLINE_HPP
#define TASK06_TIME_OUTOFLINE_HPP

class TimeBench {
    int h, m;

public:
    TimeBench(int hh = 0, int mm = 0) : h(hh), m(mm) {}

    // Inline member versions (defined inside class)
    int minutesToMember(const TimeBench& other) const {
        int diff = (h * 60 + m) - (other.h * 60 + other.m);
        return diff < 0 ? -diff : diff;
    }

    bool isEarlierMember(const TimeBench& other) const {
        return (h != other.h) ? (h < other.h) : (m < other.m);
    }

    // Inline getters for getter-based alternative
    int hour() const { return h; }
    int minute() const { return m; }

    // Inline friend declarations
    friend int minutesBetweenInline(const TimeBench& a, const TimeBench& b);
    friend bool earlierInline(const TimeBench& a, const TimeBench& b);

    // Out-of-line friend declarations (defined in separate translation unit)
    friend int minutesBetweenOutOfLine(const TimeBench& a, const TimeBench& b);
    friend bool earlierOutOfLine(const TimeBench& a, const TimeBench& b);
};

// Namespace-scope declarations so ordinary name lookup (e.g. taking function pointers)
// finds these friend functions per C++17 lookup rules.
int minutesBetweenInline(const TimeBench& a, const TimeBench& b);
bool earlierInline(const TimeBench& a, const TimeBench& b);
int minutesBetweenOutOfLine(const TimeBench& a, const TimeBench& b);
bool earlierOutOfLine(const TimeBench& a, const TimeBench& b);

inline int minutesBetweenInline(const TimeBench& a, const TimeBench& b) {
    int diff = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return diff < 0 ? -diff : diff;
}

inline bool earlierInline(const TimeBench& a, const TimeBench& b) {
    return (a.h != b.h) ? (a.h < b.h) : (a.m < b.m);
}

inline int minutesBetweenGetters(const TimeBench& a, const TimeBench& b) {
    int diff = (a.hour() * 60 + a.minute()) - (b.hour() * 60 + b.minute());
    return diff < 0 ? -diff : diff;
}

inline bool earlierGetters(const TimeBench& a, const TimeBench& b) {
    return (a.hour() != b.hour()) ? (a.hour() < b.hour()) : (a.minute() < b.minute());
}

#endif // TASK06_TIME_OUTOFLINE_HPP
