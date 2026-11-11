#include <iostream>

// 1. Friend functions design
class TimeFriend {
    int h, m;

public:
    TimeFriend(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetweenFriend(const TimeFriend& a, const TimeFriend& b);
    friend bool earlierFriend(const TimeFriend& a, const TimeFriend& b);
};

inline int minutesBetweenFriend(const TimeFriend& a, const TimeFriend& b) {
    int diff = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return diff < 0 ? -diff : diff;
}

inline bool earlierFriend(const TimeFriend& a, const TimeFriend& b) {
    return (a.h != b.h) ? (a.h < b.h) : (a.m < b.m);
}

// 2. Member functions design
class TimeMember {
    int h, m;

public:
    TimeMember(int hh, int mm) : h(hh), m(mm) {}

    int minutesTo(const TimeMember& other) const {
        int diff = (h * 60 + m) - (other.h * 60 + other.m);
        return diff < 0 ? -diff : diff;
    }

    bool isEarlier(const TimeMember& other) const {
        return (h != other.h) ? (h < other.h) : (m < other.m);
    }
};

// 3. Public getters design
class TimeGetters {
    int h, m;

public:
    TimeGetters(int hh, int mm) : h(hh), m(mm) {}
    int hour() const { return h; }
    int minute() const { return m; }
};

inline int minutesBetweenGetters(const TimeGetters& a, const TimeGetters& b) {
    int diff = (a.hour() * 60 + a.minute()) - (b.hour() * 60 + b.minute());
    return diff < 0 ? -diff : diff;
}

inline bool earlierGetters(const TimeGetters& a, const TimeGetters& b) {
    return (a.hour() != b.hour()) ? (a.hour() < b.hour()) : (a.minute() < b.minute());
}

int main() {
    TimeFriend f1(9, 45), f2(13, 10);
    TimeMember m1(9, 45), m2(13, 10);
    TimeGetters g1(9, 45), g2(13, 10);

    std::cout << std::boolalpha;
    std::cout << "Friend design : minutesBetween=" << minutesBetweenFriend(f1, f2)
              << ", earlier(13:10, 09:45)=" << earlierFriend(f2, f1) << '\n';
    std::cout << "Member design : minutesTo     =" << m1.minutesTo(m2)
              << ", isEarlier(13:10, 09:45)=" << m2.isEarlier(m1) << '\n';
    std::cout << "Getters design: minutesBetween=" << minutesBetweenGetters(g1, g2)
              << ", earlier(13:10, 09:45)=" << earlierGetters(g2, g1) << '\n';

    return 0;
}
