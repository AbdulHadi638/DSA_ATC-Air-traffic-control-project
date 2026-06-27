#pragma once
#include <iostream>
#include <iomanip>
#include "Plane.h"
using namespace std;

const int HEAP_CAPACITY = 100;

struct MinHeap {
    Plane* heap[HEAP_CAPACITY];
    int    size;

    MinHeap() { size = 0; }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swapNodes(int i, int j);
    void heapifyUp(int i);
    void heapifyDown(int i);
    void insert(Plane* p);
    Plane* extractMin();
    bool setEmergency(string id);
    bool isEmpty();
    void display();
};