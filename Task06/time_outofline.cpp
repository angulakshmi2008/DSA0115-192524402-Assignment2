#include "time_outofline.hpp"

int minutesBetweenOutOfLine(const TimeBench& a, const TimeBench& b) {
    int diff = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return diff < 0 ? -diff : diff;
}

bool earlierOutOfLine(const TimeBench& a, const TimeBench& b) {
    return (a.h != b.h) ? (a.h < b.h) : (a.m < b.m);
}
