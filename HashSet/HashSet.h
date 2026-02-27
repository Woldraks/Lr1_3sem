#pragma once
#include <string>
#include "../Array/Array.h"
using namespace std;

struct HashSet {
    Array* table;           // Используем Array из 1 лабы
    bool* isOccupied;       // отдельный массив для флагов
    bool* isDeleted;
    int capacity;
    int size;
    int collisions;
};

HashSet* CreateHashSet(int initialCapacity);
void SetAdd(HashSet* set, string key);
void SetDel(HashSet* set, string key);
bool SetAt(HashSet* set, string key);
void SetClear(HashSet* set);
HashSet* SetUnion(HashSet* set1, HashSet* set2);        // объединение
HashSet* SetIntersection(HashSet* set1, HashSet* set2); // пересечение
HashSet* SetDifference(HashSet* set1, HashSet* set2);   // разность
int SetSize(HashSet* set);
void SetPrint(HashSet* set);
void DestroyHashSet(HashSet* set);
