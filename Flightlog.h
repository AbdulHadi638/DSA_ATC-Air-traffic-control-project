#pragma once
#include <iostream>
#include <list>
#include <string>
#include <ctime>
using namespace std;

struct FlightLog {
    list<pair<string, string>> events;
    int      eventCount;

    FlightLog() : eventCount(0) {}

    string getTime();
    void addEvent(string event);
    void display();
};
