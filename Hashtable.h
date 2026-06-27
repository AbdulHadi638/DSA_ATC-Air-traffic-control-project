#pragma once
#include <list>
#include <string>
#include "Plane.h"
using namespace std;

const int HASH_SIZE = 101;

struct HashTable {
    list<pair<string, Plane*>> table[HASH_SIZE];

    int hashFn(string key);
    void insert(Plane* p);
    Plane* lookup(string id);
    void remove(string id);
};
