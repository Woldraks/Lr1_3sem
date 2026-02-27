#pragma once
#include <string>
#include <functional>
using namespace std;

struct HashTableEntry {
    string key;
    string value;
    bool isOccupied;
    bool isDeleted;
};

struct HashTable {
    HashTableEntry* table;
    int capacity;
    int size;
    int collisions;
    int probeCount;
};

struct ChainNode {
    string key;
    string value;
    ChainNode* next;
};

struct ChainedHashTable {
    ChainNode** buckets;
    int capacity;
    int size;
    int collisions;
};

HashTable* CreateOpenAddressingTable(int capacity);
ChainedHashTable* CreateChainedTable(int capacity);

void OpenAddressingInsert(HashTable* ht, string key, string value);
string OpenAddressingGet(HashTable* ht, string key);

void ChainedInsert(ChainedHashTable* ht, string key, string value);
string ChainedGet(ChainedHashTable* ht, string key);

void runEmpiricalAnalysis(int numElements, int numSearches);
