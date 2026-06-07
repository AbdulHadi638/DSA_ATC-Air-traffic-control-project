/*

   AIR TRAFFIC CONTROL (ATC) SIMULATION SYSTEM
   DSA Project
   CE-4B
   M.Abdul Hadi (24I-6538)
   Abdullah Muhammad (24I-6560)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

const int HASH_SIZE = 101;
const int HEAP_CAPACITY = 100;
const int LANDINGS_BEFORE_TAKEOFF = 3;

struct Plane {
    string flightID;
    float distance = 0.0;
    int priority = 0;
    bool isEmergency = false;
    string status = "";
};

struct MinHeap {
    Plane* heap[HEAP_CAPACITY];
    int    size;

    MinHeap() { size = 0; }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j) {
        Plane* temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)]->priority > heap[i]->priority) {
            swapNodes(i, parent(i));
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int l = leftChild(i);
        int r = rightChild(i);
        if (l < size && heap[l]->priority < heap[smallest]->priority)
            smallest = l;
        if (r < size && heap[r]->priority < heap[smallest]->priority)
            smallest = r;
        if (smallest != i) {
            swapNodes(i, smallest);
            heapifyDown(smallest);
        }
    }

    void insert(Plane* p) {
        if (size >= HEAP_CAPACITY) {
            cout << "  [!] Airspace full! Cannot add more planes.\n";
            return;
        }
        heap[size++] = p;
        heapifyUp(size - 1);
    }

    
    Plane* extractMin() {
        if (size == 0) return nullptr;
        Plane* top = heap[0];

        size--;
        heap[0] = heap[size];
        heapifyDown(0);
        return top;
    }
    bool setEmergency(string id) {
        for (int i = 0; i < size; i++) {
            if (heap[i]->flightID == id) {
                heap[i]->priority = 0;
                heap[i]->isEmergency = true;
                heapifyUp(i);
                return true;
            }
        }
        return false;
    }

    bool isEmpty() { return size == 0; }

    void display() {
        if (size == 0) {
            cout << "  (no planes in airspace)\n";
            return;
        }

        cout << "\nFlight\t\tDistance\tPriority\tStatus\n";
        cout << "  -------------------------------------------------------\n";
        for (int i = 0; i < size; i++) {
            cout << "  ";
            cout << heap[i]->flightID << "\t\t";
            cout << heap[i]->distance << "\t\t";
            cout << heap[i]->priority << "\t\t";
            if (heap[i]->isEmergency)
                cout << "EMERGENCY\n";
            else
                cout << heap[i]->status << "\n";
        }
       
    }
};


struct QNode {
    Plane* plane;
    QNode* next;
    QNode(Plane* p) : plane(p), next(nullptr) {}
};

struct TakeoffQueue {
    QNode* front;
    QNode* rear;
    int    count;

    TakeoffQueue() : front(nullptr), rear(nullptr), count(0) {}

    void enqueue(Plane* p) {
        QNode* node = new QNode(p);
        if (!rear) {
            front = rear = node;
        }
        else {
            rear->next = node;
            rear = node;
        }
        count++;
    }

    Plane* dequeue() {
        if (!front) return nullptr;
        QNode* temp = front;
        Plane* p = temp->plane;
        front = front->next;
        if (!front) { rear = nullptr; }
        delete temp;
        count--;
        return p;
    }

    bool isEmpty() { return front == nullptr; }

    void display() {
        if (!front) {
            cout << "  no planes waiting for takeoff\n";
            return;
        }
        cout << " The  Takeoff order: ";
        QNode* curr = front;
        while (curr) {
            cout << curr->plane->flightID;
            if (curr->next) cout << " -> ";
            curr = curr->next;
        }
        cout << "\n";
    }
};


struct HashNode {
    string    key;
    Plane* value;
    HashNode* next;
    HashNode(string k, Plane* v) : key(k), value(v), next(nullptr) {}
};

struct HashTable {
    HashNode* table[HASH_SIZE];

    HashTable() {
        for (int i = 0; i < HASH_SIZE; i++) table[i] = nullptr;
    }

    int hashFn(string key) {
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash += key[i];
        }
        return hash % HASH_SIZE;
    }

    void insert(Plane* p) {
        int idx = hashFn(p->flightID);
        HashNode* node = new HashNode(p->flightID, p);
        node->next = table[idx];
        table[idx] = node;
    }

    Plane* lookup(string id) {
        int idx = hashFn(id);
        HashNode* curr = table[idx];
        while (curr) {
            if (curr->key == id) return curr->value;
            curr = curr->next;
        }
        return nullptr;
    }

    void remove(string id) {
        int idx = hashFn(id);
        HashNode* curr = table[idx];
        HashNode* prev = nullptr;
        while (curr) {
            if (curr->key == id) {
                if (prev) prev->next = curr->next;
                else      table[idx] = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
};
struct LogNode {
    string   timestamp;
    string   event;
    LogNode* next;
    LogNode(string t, string e) : timestamp(t), event(e), next(nullptr) {}
};

struct FlightLog {
    LogNode* head;
    LogNode* tail;
    int      eventCount;

    FlightLog() : head(nullptr), tail(nullptr), eventCount(0) {}

    string getTime() {
        time_t now = time(0);
        tm t;
        localtime_s(&t, &now);
        char buf[10];
        sprintf_s(buf, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);
        return string(buf);
    }

    void addEvent(string event) {
        LogNode* node = new LogNode(getTime(), event);
        if (!tail) {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
        eventCount++;
    }

    void display() {
        if (!head) {
            cout << "  (no events logged yet)\n";
            return;
        }

        cout << "\n  Time\t\tEvent\n";
        cout << "  -------------------------------------------------------\n";

        LogNode* curr = head;
        while (curr) {
            cout << "  " << curr->timestamp << "\t" << curr->event << "\n";
            curr = curr->next;
        }

        cout << "  -------------------------------------------------------\n";
        cout << "  Total events: " << eventCount << "\n";
    }
};

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

    ATCSystem() {
        landingCount = 0;
        cycleNumber = 0;
        totalLandings = 0;
        totalTakeoffs = 0;
        totalEmergencies = 0;
    }


    void loadFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "\n  [!] Could not open file: " << filename << "\n";

            return;
        }

        string line;
        string section = "";

        while (getline(file, line)) {
            if (line.empty()) continue;


            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (line == "AIR") { section = "AIR";    continue; }
            if (line == "GROUND") { section = "GROUND"; continue; }

            if (section == "AIR") {
                int spacePos = (int)line.find(' ');
                if (spacePos == (int)string::npos) continue;
                string id = line.substr(0, spacePos);
                float dist = stof(line.substr(spacePos + 1));

                Plane* p = new Plane();
                p->flightID = id;
                p->distance = dist;
                p->priority = (int)(dist * 10);
                p->isEmergency = false;
                p->status = "airborne";

                airQueue.insert(p);
                registry.insert(p);
                log.addEvent("Plane " + id + " entered airspace at " + to_string(dist) + " km");
            }
            else if (section == "GROUND") {
                string id = line;

                Plane* p = new Plane();
                p->flightID = id;
                p->distance = 0;
                p->priority = 999;
                p->isEmergency = false;
                p->status = "waiting";

                groundQueue.enqueue(p);
                registry.insert(p);
                log.addEvent("Plane " + id + " added to takeoff queue");
            }
        }

        file.close();
        cout << "  [OK] Data loaded from file: " << filename << "\n";
    }




    void addAirPlane() {
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
        cout << "  [OK] Plane " << id << " added to airspace.\n";
    }


    void addGroundPlane() {
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
        cout << "  [OK] Plane " << id << " added to takeoff queue.\n";
    }


    void runCycle() {
        cycleNumber++;
        cout << "\n   CYCLE " << cycleNumber << "\n";


        if (landingCount >= LANDINGS_BEFORE_TAKEOFF && !groundQueue.isEmpty()) {
            Plane* p = groundQueue.dequeue();
            p->status = "takeoff";
            cout << "  [TAKEOFF] " << p->flightID << " cleared for takeoff after "
                << LANDINGS_BEFORE_TAKEOFF << " landings.\n";
            log.addEvent("TAKEOFF: " + p->flightID + " cleared for takeoff");
            registry.remove(p->flightID);
            landingCount = 0;

            totalTakeoffs++;
            return;
        }

        if (!airQueue.isEmpty()) {
            Plane* p = airQueue.extractMin();
            p->status = "landed";

            if (p->isEmergency) {
                cout << "  [EMERGENCY LANDING] " << p->flightID
                    << " landed immediately! (was " << p->distance << " km away)\n";
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
            cout << "  [TAKEOFF] " << p->flightID << " cleared for takeoff (no air traffic).\n";
            log.addEvent("TAKEOFF: " + p->flightID + " (no air traffic)");
            registry.remove(p->flightID);
            totalTakeoffs++;
        }
        else {
            cout << "  [INFO] No planes in airspace or ground queue. Airport is clear.\n";
            log.addEvent("CYCLE " + to_string(cycleNumber) + ": No operations");
        }
    }


    void declareEmergency() {
        string id;
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
            cout << "  [!!] EMERGENCY DECLARED for " << id << "\n";
            cout << "  [!!] Priority set to 0 -- will land next cycle.\n";
            log.addEvent("EMERGENCY DECLARED: " + id + " given priority 0");
        }
        else {
            cout << "  [!] Could not find " << id << " in airspace heap.\n";
        }
    }


    void searchPlane() {
        string id;
        cout << "\n  Enter Flight ID to search: ";
        cin >> id;

        Plane* p = registry.lookup(id);
        if (!p) {
            cout << "  [!] Flight " << id << " not found.\n";
            return;
        }

        cout << "\n   Plane Info :\n";
        cout << "  Flight ID  : " << p->flightID << "\n";
        cout << "  Status     : " << p->status << "\n";
        cout << "  Distance   : " << p->distance << " km\n";
        cout << "  Priority   : " << p->priority << "\n";
        cout << "  Emergency  : " << (p->isEmergency ? "YES" : "NO") << "\n";

    }

    void viewAirspace() { cout << "\n   PLANES IN AIRSPACE :\n";     airQueue.display(); }
    void viewGroundQueue() { cout << "\n   TAKEOFF QUEUE (GROUND) \n"; groundQueue.display(); }
    void viewLog() { cout << "\n  FLIGHT EVENT LOG \n";        log.display(); }

    void dailyReport() {
        cout << "         DAILY REPORT\n";

        cout << "  Total Cycles Run    : " << cycleNumber << "\n";
        cout << "  Total Landings      : " << totalLandings << "\n";
        cout << "  Total Takeoffs      : " << totalTakeoffs << "\n";
        cout << "  Total Emergencies   : " << totalEmergencies << "\n";
        cout << "  Planes in Airspace  : " << airQueue.size << "\n";
        cout << "  Planes on Ground    : " << groundQueue.count << "\n";
        cout << "  Events Logged       : " << log.eventCount << "\n";

    }

    void runMultipleCycles() {
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
};

void printBanner() {
    cout << "\n";
    cout << "  ----------------------------------------------\n";
    cout << "  |     AIR TRAFFIC CONTROL SIMULATION SYSTEM    |\n";
    cout << " ----------------------------------------------\n\n";
}

void printMenu() {
    cout << "\n------------------------------------------\n";
    cout << "  |              MAIN MENU                   |\n";
    cout << " ------------------------------------------\n";
    cout << "    1. Run One Cycle                        \n";
    cout << "    2. Run Multiple Cycles                  \n";
    cout << "    3. Declare Emergency                    \n";
    cout << "    4. Add New Plane (Airborne)             \n";
    cout << "    5. Add New Plane (Ground/Takeoff)       \n";
    cout << "    6. View Airspace (Planes in Air)        \n";
    cout << "    7. View Takeoff Queue (Ground)          \n";
    cout << "    8. Search Plane by Flight ID            \n";
    cout << "    9. View Flight Log                      \n";
    cout << "    10. Daily Report                        \n";
    cout << "    0. Exit                                 \n";

    cout << "  Enter choice: ";
}


int main() {
    printBanner();

    ATCSystem atc;

    cout << "  Loading airport data...\n";
    atc.loadFromFile("airport_data_test_run.txt");

    cout << "\n  [INFO] Takeoff rule: 1 takeoff after every " << LANDINGS_BEFORE_TAKEOFF << " landings.\n";


    int choice;
    do {
        printMenu();
        cin >> choice;

        switch (choice) {
        case 1:  atc.runCycle();          break;
        case 2:  atc.runMultipleCycles(); break;
        case 3:  atc.declareEmergency();  break;
        case 4:  atc.addAirPlane();       break;
        case 5:  atc.addGroundPlane();    break;
        case 6:  atc.viewAirspace();      break;
        case 7:  atc.viewGroundQueue();   break;
        case 8:  atc.searchPlane();       break;
        case 9:  atc.viewLog();           break;
        case 10: atc.dailyReport();       break;
        case 0:
            cout << "\n  [ATC] System shutting down. Goodbye!\n\n";
            break;
        default:
            cout << "\n  [!] Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}