#include "ATCSystem.h"

void printBanner() {
    cout << "\n";
    cout << "============================================================\n";
    cout << "        JFK AIR TRAFFIC CONTROL SYSTEM (ATC)\n";
    cout << "============================================================\n";
    cout << "   Real-Time Flight Monitoring & Runway Management\n";
    cout << "------------------------------------------------------------\n\n";
}

void printMenu() {
    cout << "\n============================================================\n";
    cout << "                     MAIN CONTROL PANEL\n";
    cout << "============================================================\n";

    cout << "  [ OPERATIONS ]\n";
    cout << "   1  -> Run One Cycle\n";
    cout << "   2  -> Run Multiple Cycles\n";
    cout << "   3  -> Declare Emergency\n\n";

    cout << "  [ AIRCRAFT MANAGEMENT ]\n";
    cout << "   4  -> Add Airborne Plane\n";
    cout << "   5  -> Add Ground Plane (Takeoff Queue)\n\n";

    cout << "  [ MONITORING ]\n";
    cout << "   6  -> View Airspace\n";
    cout << "   7  -> View Takeoff Queue\n";
    cout << "   8  -> Search Flight\n";
    cout << "   9  -> View Flight Log\n";
    cout << "  10  -> Daily Report\n\n";

    cout << "------------------------------------------------------------\n";
    cout << "   0  -> Exit System\n";
    cout << "============================================================\n";

    cout << "  ATC Command > ";
}

int main() {
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        FOREGROUND_GREEN | FOREGROUND_INTENSITY
    );
    printBanner();

    ATCSystem atc;

    cout << "  Loading airport data...\n";
    atc.loadFromFile("flights.csv");
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