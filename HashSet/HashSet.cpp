#include "HashSet.h"
#include "../Array/Array.h"
#include <iostream>
#include <string>
using namespace std;

int hashFunction(const string& key, int capacity) {
    unsigned long hash = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % capacity;
}

HashSet* CreateHashSet(int initialCapacity) {
    HashSet* set = new HashSet;
    set->capacity = initialCapacity;
    set->size = 0;
    set->collisions = 0;
    set->table = CreateArray(initialCapacity);
    set->isOccupied = new bool[initialCapacity];
    set->isDeleted = new bool[initialCapacity];
    
    for (int i = 0; i < initialCapacity; i++) {
        set->isOccupied[i] = false;
        set->isDeleted[i] = false;
        Push(set->table, "");
    }
    
    return set;
}

void resizeHashSet(HashSet* set) {
    int oldCapacity = set->capacity;
    Array* oldTable = set->table;
    bool* oldOccupied = set->isOccupied;
    bool* oldDeleted = set->isDeleted;
    
    set->capacity *= 2;
    set->size = 0;
    set->collisions = 0;
    set->table = CreateArray(set->capacity);
    set->isOccupied = new bool[set->capacity];
    set->isDeleted = new bool[set->capacity];
    
    for (int i = 0; i < set->capacity; i++) {
        set->isOccupied[i] = false;
        set->isDeleted[i] = false;
        Push(set->table, "");
    }
    
    for (int i = 0; i < oldCapacity; i++) {
        if (oldOccupied[i] && !oldDeleted[i]) {
            string key = GetByIndex(oldTable, i);
            SetAdd(set, key);
        }
    }
    
    Destroy(oldTable);
    delete[] oldOccupied;
    delete[] oldDeleted;
}

void SetAdd(HashSet* set, string key) {
    if ((float)set->size / set->capacity > 0.75) {
        resizeHashSet(set);
    }
    
    int index = hashFunction(key, set->capacity);
    int originalIndex = index;
    bool collided = false;
    
    while (set->isOccupied[index] && !set->isDeleted[index]) {
        if (GetByIndex(set->table, index) == key) {
            return;
        }
        index = (index + 1) % set->capacity;
        collided = true;
        if (index == originalIndex) {
            resizeHashSet(set);
            index = hashFunction(key, set->capacity);
            originalIndex = index;
            collided = false;
        }
    }
    
    if (collided) {
        set->collisions++;
    }
    
    ReplaceByIndex(set->table, index, key);
    set->isOccupied[index] = true;
    set->isDeleted[index] = false;
    set->size++;
}

void SetDel(HashSet* set, string key) {
    int index = hashFunction(key, set->capacity);
    int originalIndex = index;
    
    while (set->isOccupied[index]) {
        if (!set->isDeleted[index] && GetByIndex(set->table, index) == key) {
            set->isDeleted[index] = true;
            set->size--;
            return;
        }
        index = (index + 1) % set->capacity;
        if (index == originalIndex) break;
    }
}

bool SetAt(HashSet* set, string key) {
    int index = hashFunction(key, set->capacity);
    int originalIndex = index;
    
    while (set->isOccupied[index]) {
        if (!set->isDeleted[index] && GetByIndex(set->table, index) == key) {
            return true;
        }
        index = (index + 1) % set->capacity;
        if (index == originalIndex) break;
    }
    return false;
}

void SetClear(HashSet* set) {
    for (int i = 0; i < set->capacity; i++) {
        set->isOccupied[i] = false;
        set->isDeleted[i] = false;
        ReplaceByIndex(set->table, i, "");
    }
    set->size = 0;
    set->collisions = 0;
}

int SetSize(HashSet* set) {
    return set->size;
}

void SetPrint(HashSet* set) {
    cout << "{";
    bool first = true;
    for (int i = 0; i < set->capacity; i++) {
        if (set->isOccupied[i] && !set->isDeleted[i]) {
            if (!first) {
                cout << ", ";
            }
            cout << GetByIndex(set->table, i);
            first = false;
        }
    }
    cout << "}" << endl;
}

HashSet* SetUnion(HashSet* set1, HashSet* set2) {
    HashSet* result = CreateHashSet(set1->capacity + set2->capacity);
    
    // Добавляем все элементы из первого множества
    for (int i = 0; i < set1->capacity; i++) {
        if (set1->isOccupied[i] && !set1->isDeleted[i]) {
            SetAdd(result, GetByIndex(set1->table, i));
        }
    }
    
    // Добавляем все элементы из второго множества
    for (int i = 0; i < set2->capacity; i++) {
        if (set2->isOccupied[i] && !set2->isDeleted[i]) {
            SetAdd(result, GetByIndex(set2->table, i));
        }
    }
    
    return result;
}

HashSet* SetIntersection(HashSet* set1, HashSet* set2) {
    HashSet* result = CreateHashSet((set1->capacity < set2->capacity) ? set1->capacity : set2->capacity);
    
    // Ищем элементы из set1, которые есть в set2
    for (int i = 0; i < set1->capacity; i++) {
        if (set1->isOccupied[i] && !set1->isDeleted[i]) {
            string key = GetByIndex(set1->table, i);
            if (SetAt(set2, key)) {
                SetAdd(result, key);
            }
        }
    }
    
    return result;
}

HashSet* SetDifference(HashSet* set1, HashSet* set2) {
    HashSet* result = CreateHashSet(set1->capacity);
    
    // Ищем элементы из set1, которых нет в set2
    for (int i = 0; i < set1->capacity; i++) {
        if (set1->isOccupied[i] && !set1->isDeleted[i]) {
            string key = GetByIndex(set1->table, i);
            if (!SetAt(set2, key)) {
                SetAdd(result, key);
            }
        }
    }
    
    return result;
}

void DestroyHashSet(HashSet* set) {
    Destroy(set->table);
    delete[] set->isOccupied;
    delete[] set->isDeleted;
    delete set;
}
