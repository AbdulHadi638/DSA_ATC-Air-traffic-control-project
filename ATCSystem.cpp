#include "ATCSystem.h"


HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int c) { SetConsoleTextAttribute(hCon, c); }

void beginInfo() {
    setColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\n========================================\n";
}

void endResult() {
    cout << "========================================\n";
    setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
ATCSystem::ATCSystem() {
    landingCount = 0;
    cycleNumber = 0;
    totalLandings = 0;
    totalTakeoffs = 0;
    totalEmergencies = 0;
}

void ATCSystem::loadFromFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "\n  [!] Could not open file: " << filename << "\n";
        return;
    }

    string line;


    getline(file, line);

    int loaded = 0;

    while (getline(file, line) && loaded < 80) {

        string cols[31];
        string temp;
        stringstream ss(line);

        int i = 0;
        while (getline(ss, temp, ',') && i < 31) {
            cols[i++] = temp;
        }

        if (i < 18)
            continue;

        string airline = cols[4];
        string flightNumber = cols[5];

        string origin = cols[7];
        string destination = cols[8];

        string distanceStr = cols[17];

        if (origin != "JFK" && destination != "JFK")
            continue;

        Plane* p = new Plane();

        p->flightID = airline + flightNumber;

        p->isEmergency = false;
        p->originAirport = origin;
        p->destinationAirport = destination;


        if (destination == "JFK") {

            if (distanceStr.empty()) {
                delete p;
                continue;
            }

            try {
                p->distance = stof(distanceStr);
            }
            catch (...) {
                delete p;
                continue;
            }

            p->priority = (int)(p->distance * 10);
            p->status = "airborne";

            airQueue.insert(p);
            registry.insert(p);

            log.addEvent(
                "Arrival flight " +
                p->flightID +
                " entered airspace"
            );

            loaded++;
        }


        else if (origin == "JFK") {

            p->distance = 0;
            p->priority = 999;
            p->status = "waiting";

            groundQueue.enqueue(p);
            registry.insert(p);

            log.addEvent(
                "Departure flight " +
                p->flightID +
                " added to takeoff queue"
            );

            loaded++;
        }
    }

    file.close();

    cout << "\n  [OK] Loaded " << loaded
        << " JFK-related flights from CSV.\n";
    exportState();
}

void ATCSystem::addAirPlane() {
    string id;
    float dist;
    cout << "\n  Enter Flight ID   : ";
    cin >> id;

    if (registry.lookup(id)) {
        cout << "  [!] Flight " << id << " already exists in system.\n";
        return;
    }

    cout << "  Enter Distance (km): ";
    cin >> dist;

    Plane* p = new Plane();
    p->flightID = id;

    p->distance = dist;
    p->priority = (int)(dist * 10);
    p->isEmergency = false;
    p->status = "airborne";

    airQueue.insert(p);
    registry.insert(p);

    log.addEvent("NEW: Plane " + id + " entered airspace at " + to_string(dist) + " km");

    beginInfo();
    cout << "  [OK] Plane " << id << " added to airspace.\n";
    endResult();
}

void ATCSystem::addGroundPlane() {
    string id;
    cout << "\n  Enter Flight ID: ";
    cin >> id;

    if (registry.lookup(id)) {
        cout << "  [!] Flight " << id << " already exists in system.\n";
        return;
    }

    Plane* p = new Plane();
    p->flightID = id;
    p->distance = 0;
    p->priority = 999;
    p->isEmergency = false;
    p->status = "waiting";

    groundQueue.enqueue(p);
    registry.insert(p);
    log.addEvent("NEW: Plane " + id + " added to takeoff queue");

    beginInfo();
    cout << "  [OK] Plane " << id << " added to takeoff queue.\n";
    endResult();
}

void ATCSystem::runCycle() {
    cycleNumber++;

    beginInfo();
    cout << "\n   CYCLE " << cycleNumber << "\n";


    if (landingCount >= LANDINGS_BEFORE_TAKEOFF && !groundQueue.isEmpty()) {
        Plane* p = groundQueue.dequeue();
        p->status = "takeoff";
        cout << "  [TAKEOFF] " << p->flightID
            << " departing to " << p->destinationAirport
            << " cleared for takeoff after "
            << LANDINGS_BEFORE_TAKEOFF << " landings.\n";
        log.addEvent("TAKEOFF: " + p->flightID + " cleared for takeoff");
        registry.remove(p->flightID);
        landingCount = 0;

        totalTakeoffs++;
        endResult();
        exportState();
        return;
    }

    if (!airQueue.isEmpty()) {
        Plane* p = airQueue.extractMin();
        p->status = "landed";

        if (p->isEmergency) {
            cout << "  [EMERGENCY LANDING] " << p->flightID
                << " arriving from " << p->originAirport
                << " landed immediately! (was "
                << p->distance << " km away)\n";
            log.addEvent("EMERGENCY LAND: " + p->flightID + " landed immediately");

            totalEmergencies++;
        }
        else {
            cout << "  [LANDING] " << p->flightID
                << " cleared to land. (distance: " << p->distance << " km)\n";
            log.addEvent("LANDED: " + p->flightID + " landed successfully");
        }

        registry.remove(p->flightID);
        landingCount++;
        totalLandings++;
    }
    else if (!groundQueue.isEmpty()) {
        Plane* p = groundQueue.dequeue();
        p->status = "takeoff";
        cout << "  [TAKEOFF] " << p->flightID
            << " departing to " << p->destinationAirport
            << " cleared for takeoff (no air traffic).\n";
        log.addEvent("TAKEOFF: " + p->flightID + " (no air traffic)");
        registry.remove(p->flightID);
        totalTakeoffs++;
    }
    else {
        cout << "  [INFO] No planes in airspace or ground queue. Airport is clear.\n";
        log.addEvent("CYCLE " + to_string(cycleNumber) + ": No operations");
    }
    endResult();
    exportState();
}

void ATCSystem::declareEmergency() {
    string id;

    cout << "\nAircraft currently in airspace:\n";

    for (int i = 0; i < airQueue.size; i++)
    {
        cout << airQueue.heap[i]->flightID
            << " (" << airQueue.heap[i]->originAirport
            << " -> JFK)\n";
    }
    cout << "\n  Enter Flight ID to declare emergency: ";
    cin >> id;


    Plane* p = registry.lookup(id);
    if (!p) {
        cout << "  [!] Flight " << id << " not found in system.\n";
        return;
    }
    if (p->status != "airborne") {
        cout << "  [!] " << id << " is not currently airborne (status: " << p->status << ").\n";
        return;
    }
    if (p->isEmergency) {
        cout << "  [!] " << id << " is already declared as emergency.\n";
        return;
    }

    if (airQueue.setEmergency(id)) {
        beginInfo();
        cout << "  [EMERGENCY] " << id << " declared emergency.\n";
        cout << "  [ATC] Emergency traffic acknowledged. "
            << id << " is now first in the landing sequence.\n";
        endResult();
        log.addEvent("EMERGENCY DECLARED: " + id + " granted immediate landing priority");
    }
    else {
        cout << "  [!] Could not find " << id << " in airspace heap.\n";
    }
    exportState();
}

void ATCSystem::searchPlane() {
    string id;
    cout << "\n  Enter Flight ID to search: ";
    cin >> id;

    Plane* p = registry.lookup(id);
    if (!p) {
        cout << "  [!] Flight " << id << " not found.\n";
        return;
    }

    beginInfo();
    cout << "\n   Plane Info :\n";
    cout << "  Flight ID  : " << p->flightID << "\n";
    cout << "  Status     : " << p->status << "\n";
    cout << "  Distance   : " << p->distance << " km\n";
    cout << "  Priority   : " << p->priority << "\n";
    cout << "  Emergency  : " << (p->isEmergency ? "YES" : "NO") << "\n";
    endResult();
}

void ATCSystem::viewAirspace() {
    beginInfo();
    cout << "\n   PLANES IN AIRSPACE :\n";
    airQueue.display();
    endResult();
}

void ATCSystem::viewGroundQueue() {
    beginInfo();
    cout << "\n   TAKEOFF QUEUE (GROUND) \n";
    groundQueue.display();
    endResult();
}

void ATCSystem::viewLog() {
    beginInfo();
    cout << "\n  FLIGHT EVENT LOG \n";
    log.display();
    endResult();
}

void ATCSystem::dailyReport() {
    beginInfo();
    cout << "         DAILY REPORT\n";

    cout << "  Total Cycles Run    : " << cycleNumber << "\n";
    cout << "  Total Landings      : " << totalLandings << "\n";
    cout << "  Total Takeoffs      : " << totalTakeoffs << "\n";
    cout << "  Total Emergencies   : " << totalEmergencies << "\n";
    cout << "  Planes in Airspace  : " << airQueue.size << "\n";
    cout << "  Planes on Ground    : " << groundQueue.count << "\n";
    cout << "  Events Logged       : " << log.eventCount << "\n";
    endResult();
}

void ATCSystem::exportState() {
    ofstream f("state.json");
    if (!f.is_open()) return;




    f << "{\n";
    f << "  \"cycleNumber\": " << cycleNumber << ",\n";
    f << "  \"totalLandings\": " << totalLandings << ",\n";
    f << "  \"totalTakeoffs\": " << totalTakeoffs << ",\n";
    f << "  \"totalEmergencies\": " << totalEmergencies << ",\n";
    f << "  \"airQueueSize\": " << airQueue.size << ",\n";
    f << "  \"groundQueueSize\": " << groundQueue.count << ",\n";

    f << "  \"airQueue\": [\n";
    int showCount = min(5, airQueue.size);
    for (int i = 0; i < showCount; i++) {
        Plane* p = airQueue.heap[i];
        float angle = (i * 72.0f) * 3.14159f / 180.0f;
        float r = min(0.9f, p->distance / 6000.0f);
        float x = r * cos(angle);
        float y = r * sin(angle);
        f << "    {";
        f << "\"id\": \"" << p->flightID << "\", ";
        f << "\"distance\": " << (int)p->distance << ", ";
        f << "\"emergency\": " << (p->isEmergency ? "true" : "false") << ", ";
        f << "\"x\": " << x << ", ";
        f << "\"y\": " << y;
        f << "}";
        if (i < showCount - 1) f << ",";
        f << "\n";
    }
    f << "  ],\n";

    f << "  \"groundQueue\": [";
    queue<Plane*> temp = groundQueue.q;
    int gc = 0;
    while (!temp.empty() && gc < 3) {
        if (gc > 0) f << ", ";
        f << "\"" << temp.front()->flightID << "\"";
        temp.pop();
        gc++;
    }
    f << "]\n}\n";
    f.close();
}

void ATCSystem::runMultipleCycles() {
    int n;
    cout << "\n  How many cycles to run? ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        if (airQueue.isEmpty() && groundQueue.isEmpty()) {
            cout << "\n  [INFO] Airport is clear. No more operations.\n";
            break;
        }
        runCycle();
    }
}