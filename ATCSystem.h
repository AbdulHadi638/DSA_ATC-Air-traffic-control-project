#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <windows.h>
#include "MinHeap.h"
#include "TakeoffQueue.h"
#include "HashTable.h"
#include "FlightLog.h"
using namespace std;

const int LANDINGS_BEFORE_TAKEOFF = 3;


extern HANDLE hCon;

void setColor(int c);
void beginInfo();
void endResult();


struct ATCSystem {
    MinHeap      airQueue;
    TakeoffQueue groundQueue;
    HashTable    registry;
    FlightLog    log;

    int landingCount;
    int cycleNumber;
    int totalLandings;
    int totalTakeoffs;
    int totalEmergencies;

    ATCSystem();

    void loadFromFile(string filename);
    void addAirPlane();
    void addGroundPlane();
    void runCycle();
    void declareEmergency();
    void searchPlane();
    void viewAirspace();
    void viewGroundQueue();
    void viewLog();
    void dailyReport();
    void exportState();
    void runMultipleCycles();
};