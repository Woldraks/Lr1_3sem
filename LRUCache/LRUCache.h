#pragma once
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Узел для LRU кэша
struct LRUNode {
    string key;
    string value;
    LRUNode* prev;
    LRUNode* next;
    
    LRUNode(string k, string v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// Сам LRU кэш
class LRUCache {
private:
    int capacity;                    // Максимальный размер
    int size;                        // Текущий размер (добавили!)
    LRUNode* head;                    // Самый свежий (начало)
    LRUNode* tail;                    // Самый старый (конец)
    unordered_map<string, LRUNode*> cache;  // Хеш-таблица для быстрого поиска
    
    void moveToHead(LRUNode* node);   // Переместить элемент в начало
    void removeTail();                 // Удалить самый старый
    
public:
    LRUCache(int cap);                 // Конструктор
    ~LRUCache();                        // Деструктор
    
    string get(string key);             // Получить значение
    void set(string key, string value); // Положить значение
    void printCache();                  // Печать состояния
};

// Структура для команд
struct LRUCacheCommand {
    string type;   // "SET" или "GET"
    string key;
    string value;  // для SET
};

// Симулятор
class LRUCacheSimulator {
public:
    static void simulate(int cacheSize, const vector<LRUCacheCommand>& commands);
};