#include "FlightLog.h"

string FlightLog::getTime() {
    time_t now = time(0);
    tm t;
    localtime_s(&t, &now);
    char buf[10];
    sprintf_s(buf, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
    return string(buf);
}

void FlightLog::addEvent(string event) {
    events.push_back({ getTime(), event });
    eventCount++;
}

void FlightLog::display() {
    if (events.empty()) {
        cout << "  (no events logged yet)\n";
        return;
    }

    cout << "\n  Time\t\tEvent\n";
    cout << "  -------------------------------------------------------\n";

    for (auto& curr : events) {
        cout << "  " << curr.first << "\t" << curr.second << "\n";
    }

    cout << "  -------------------------------------------------------\n";
    cout << "  Total events: " << eventCount << "\n";
}