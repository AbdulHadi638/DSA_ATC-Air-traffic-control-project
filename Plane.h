#pragma once
#include <string>
using namespace std;

struct Plane
{
    string flightID;
    float distance = 0.0;
    int priority = 0;
    bool isEmergency = false;
    string status = "";

    string originAirport;
    string destinationAirport;
};
