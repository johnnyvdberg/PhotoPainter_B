#ifndef DURATION_H
#define DURATION_H

#include <stdlib.h>
#include "stdio.h"
#include <ctype.h>
#include <stdint.h>
#include <time.h>

/// Durations are stored as relative time in seconds
typedef int64_t DurationSeconds;

// Returns 1 if duration is >= 60 seconds
static inline int is_valid_duration(DurationSeconds seconds);

// Computes absolute difference between two durations (in seconds)
static inline DurationSeconds diff_durations(DurationSeconds a, DurationSeconds b);

// Adds a duration (seconds) to a given time and returns normalized `struct tm`
static inline struct tm add_duration_to_time(struct tm t, DurationSeconds duration_secs);

/// ------------------- Implementation -------------------

static inline int is_valid_duration(DurationSeconds seconds) {
    return seconds >= 60;
}

static inline DurationSeconds diff_durations(DurationSeconds a, DurationSeconds b) {
    return a > b ? a - b : b - a;
}

static inline struct tm add_duration_to_time(struct tm t, DurationSeconds duration_secs) {
    time_t base = mktime(&t);
    base += duration_secs;
    return *localtime(&base);  // Use gmtime(&base) for UTC
}

// Convert Time_data to struct tm
struct tm to_tm(Time_data t) {
    struct tm result = {0};
    result.tm_year = t.years - 1900;  // tm_year is years since 1900
    result.tm_mon  = t.months - 1;    // tm_mon is 0-11
    result.tm_mday = t.days;
    result.tm_hour = t.hours;
    result.tm_min  = t.minutes;
    result.tm_sec  = t.seconds;
    return result;
}

// Convert struct tm back to Time_data
Time_data from_tm(struct tm t) {
    Time_data result;
    result.years   = t.tm_year + 1900;
    result.months  = t.tm_mon + 1;
    result.days    = t.tm_mday;
    result.hours   = t.tm_hour;
    result.minutes = t.tm_min;
    result.seconds = t.tm_sec;
    return result;
}

#endif // DURATION_H
