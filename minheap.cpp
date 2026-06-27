#include "MinHeap.h"

void MinHeap::swapNodes(int i, int j) {
    Plane* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void MinHeap::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)]->priority > heap[i]->priority) {
        swapNodes(i, parent(i));
        i = parent(i);
    }
}

void MinHeap::heapifyDown(int i) {
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

void MinHeap::insert(Plane* p) {
    if (size >= HEAP_CAPACITY) {
        cout << "  [!] Airspace full! Cannot add more planes.\n";
        return;
    }
    heap[size++] = p;
    heapifyUp(size - 1);
}

Plane* MinHeap::extractMin() {
    if (size == 0) return nullptr;
    Plane* top = heap[0];

    size--;
    heap[0] = heap[size];
    heapifyDown(0);
    return top;
}

bool MinHeap::setEmergency(string id) {
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

bool MinHeap::isEmpty() { return size == 0; }

void MinHeap::display() {
    if (size == 0) {
        cout << "  (no aircraft currently in airspace)\n";
        return;
    }

    cout << "\nAircraft Currently in Airspace\n";
    cout << "------------------------------------------------------------\n";

    cout << left
        << setw(12) << "Flight"
        << setw(12) << "Origin"
        << setw(15) << "Distance(km)"
        << setw(15) << "Status"
        << '\n';

    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < size; i++) {
        cout << left
            << setw(12) << heap[i]->flightID
            << setw(12) << heap[i]->originAirport
            << setw(15) << (int)heap[i]->distance
            << setw(15) << (heap[i]->isEmergency
                ? "EMERGENCY"
                : heap[i]->status)
            << '\n';
    }

    cout << "------------------------------------------------------------\n";
}