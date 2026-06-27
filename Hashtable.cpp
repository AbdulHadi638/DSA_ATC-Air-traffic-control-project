#include "HashTable.h"

int HashTable::hashFn(string key) {
    int hash = 0;
    for (int i = 0; i < key.length(); i++) {
        hash += key[i];
    }
    return hash % HASH_SIZE;
}

void HashTable::insert(Plane* p) {
    int idx = hashFn(p->flightID);

    table[idx].push_front({ p->flightID, p });
}

Plane* HashTable::lookup(string id) {
    int idx = hashFn(id);

    for (auto& node : table[idx]) {
        if (node.first == id)
            return node.second;
    }

    return nullptr;
}

void HashTable::remove(string id) {
    int idx = hashFn(id);

    for (auto it = table[idx].begin();
        it != table[idx].end();
        ++it)
    {
        if (it->first == id) {
            table[idx].erase(it);
            return;
        }
    }
}