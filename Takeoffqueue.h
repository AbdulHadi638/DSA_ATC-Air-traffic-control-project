#pragma once
#include <iostream>
#include <queue>
#include "Plane.h"
using namespace std;

struct TakeoffQueue {
    queue<Plane*> q;
    int    count;

    TakeoffQueue() : count(0) {}

    void enqueue(Plane* p);
    Plane* dequeue();
    bool isEmpty();
    void display();
};
