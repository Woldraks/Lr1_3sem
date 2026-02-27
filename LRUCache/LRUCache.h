#pragma once
#include <string>
#include <vector>
#include "../LinkedList/DoublyLinkedList.h"
using namespace std;

struct LRUNode {
    string key;
    string value;
    LRUNode* prev;
    LRUNode* next;
    
    LRUNode(string k, string v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    LinkedList* list;           // Используем DoublyLinkedList из 1 лабы
    LRUNode* head;
    LRUNode* tail;
    
    void moveToHead(LRUNode* node);
    void removeTail();
    
public:
    LRUCache(int cap);
    ~LRUCache();
    
    string get(string key);
    void set(string key, string value);
    void printCache();
};

struct LRUCacheCommand {
    string type;   // "SET" или "GET"
    string key;
    string value;  // для SET
};

class LRUCacheSimulator {
public:
    static void simulate(int cacheSize, const vector<LRUCacheCommand>& commands);
};
