#include "TakeoffQueue.h"

void TakeoffQueue::enqueue(Plane* p) {
    q.push(p);
    count++;
}

Plane* TakeoffQueue::dequeue() {
    if (q.empty()) return nullptr;
    Plane* p = q.front();
    q.pop();
    count--;
    return p;
}

bool TakeoffQueue::isEmpty() { return q.empty(); }

void TakeoffQueue::display() {
    if (q.empty()) {
        cout << "  no planes waiting for takeoff\n";
        return;
    }
    cout << " The  Takeoff order: ";

    queue<Plane*> temp = q;

    while (!temp.empty()) {
        cout << temp.front()->flightID;
        temp.pop();

        if (!temp.empty()) cout << " -> ";
    }
    cout << "\n";
}