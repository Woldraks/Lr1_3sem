#include "LRUCache.h"
#include <iostream>
using namespace std;

// ===== КОНСТРУКТОР =====
LRUCache::LRUCache(int cap) : capacity(cap), size(0), head(nullptr), tail(nullptr) {

}

// ===== ДЕСТРУКТОР =====
LRUCache::~LRUCache() {
    LRUNode* current = head;
    while (current != nullptr) {
        LRUNode* next = current->next;
        delete current;
        current = next;
    }
    // 🟢 Убрали Destroy(list)!
}

// ===== ПЕРЕМЕСТИТЬ В НАЧАЛО =====
void LRUCache::moveToHead(LRUNode* node) {
    if (node == nullptr || node == head) return;
    
    // Вырезаем узел из текущей позиции
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    }
    if (node == tail) {
        tail = node->prev;
    }
    
    // Вставляем в начало
    node->next = head;
    node->prev = nullptr;
    
    if (head != nullptr) {
        head->prev = node;
    }
    head = node;
    
    if (tail == nullptr) {
        tail = node;
    }
}

// ===== УДАЛИТЬ САМЫЙ СТАРЫЙ =====
void LRUCache::removeTail() {
    if (tail == nullptr) return;
    
    // Удаляем из хеш-таблицы
    cache.erase(tail->key);
    
    LRUNode* toDelete = tail;
    
    if (tail->prev != nullptr) {
        tail = tail->prev;
        tail->next = nullptr;
    } else {
        // Был только один элемент
        head = nullptr;
        tail = nullptr;
    }
    
    delete toDelete;
    size--;
}

// ===== GET - ПОЛУЧИТЬ ЗНАЧЕНИЕ =====
string LRUCache::get(string key) {
    // 🟢 Быстрый поиск через хеш-таблицу O(1)
    auto it = cache.find(key);
    if (it == cache.end()) {
        return "-1";
    }
    
    LRUNode* node = it->second;
    moveToHead(node);  // Перемещаем в начало (свежий)
    return node->value;
}

// ===== SET - ПОЛОЖИТЬ ЗНАЧЕНИЕ =====
void LRUCache::set(string key, string value) {
    // 🟢 Проверяем через хеш-таблицу O(1)
    auto it = cache.find(key);
    
    if (it != cache.end()) {
        // Ключ уже есть - обновляем
        LRUNode* node = it->second;
        node->value = value;
        moveToHead(node);
    } else {
        // Новый ключ - создаем узел
        LRUNode* newNode = new LRUNode(key, value);
        
        // Добавляем в хеш-таблицу
        cache[key] = newNode;
        
        // Вставляем в начало списка
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        
        size++;  // 🟢 Увеличиваем счетчик
        
        // Проверяем, не превышен ли размер
        if (size > capacity) {
            removeTail();  // Удаляем самый старый
            // size уменьшится в removeTail()
        }
    }
}

// ===== ПЕЧАТЬ КЭША =====
void LRUCache::printCache() {
    cout << "LRU Cache (от свежего к старому):" << endl;
    LRUNode* current = head;
    int pos = 1;
    while (current != nullptr) {
        cout << "  [" << pos++ << "] " << current->key << " -> " << current->value << endl;
        current = current->next;
    }
    cout << "Размер: " << size << "/" << capacity << endl;
}

// ===== СИМУЛЯТОР =====
void LRUCacheSimulator::simulate(int cacheSize, const vector<LRUCacheCommand>& commands) {
    LRUCache cache(cacheSize);
    
    cout << "\n=== LRU КЭШ (емкость: " << cacheSize << ") ===" << endl;
    cout << "==========================================" << endl;
    
    for (size_t i = 0; i < commands.size(); i++) {
        cout << "\n👉 Команда " << i+1 << ": ";
        
        if (commands[i].type == "SET") {
            cout << "SET " << commands[i].key << " = " << commands[i].value << endl;
            cache.set(commands[i].key, commands[i].value);
        } 
        else if (commands[i].type == "GET") {
            string result = cache.get(commands[i].key);
            cout << "GET " << commands[i].key << " -> " << result << endl;
        }
        
        cache.printCache();
        cout << "------------------------------------------" << endl;
    }
}