#include "LRUCache.h"
#include "../LinkedList/DoublyLinkedList.h"
#include <iostream>
#include <string>
using namespace std;

LRUCache::LRUCache(int cap) : capacity(cap), head(nullptr), tail(nullptr) {
    list = CreateLinkedList();
}

LRUCache::~LRUCache() {
    LRUNode* current = head;
    while (current != nullptr) {
        LRUNode* next = current->next;
        delete current;
        current = next;
    }
    Destroy(list);
}

void LRUCache::moveToHead(LRUNode* node) {
    if (node == nullptr || node == head) return;
    
    // Удаляем node из текущей позиции
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

void LRUCache::removeTail() {
    if (tail == nullptr) return;
    
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
}

string LRUCache::get(string key) {
    LRUNode* current = head;
    while (current != nullptr) {
        if (current->key == key) {
            moveToHead(current);
            return current->value;
        }
        current = current->next;
    }
    return "-1";
}

void LRUCache::set(string key, string value) {
    // Ищем существующий элемент
    LRUNode* current = head;
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            moveToHead(current);
            return;
        }
        current = current->next;
    }
    
    // Создаем новый
    LRUNode* newNode = new LRUNode(key, value);
    
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    
    // Проверяем превышение емкости
    int count = 0;
    current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    if (count > capacity) {
        removeTail();
    }
}

void LRUCache::printCache() {
    cout << "LRU Cache (от наиболее свежего к наименее используемому):" << endl;
    LRUNode* current = head;
    int count = 0;
    while (current != nullptr) {
        cout << "  [" << count + 1 << "] " << current->key << " -> " << current->value << endl;
        current = current->next;
        count++;
    }
    cout << "Размер: " << count << "/" << capacity << endl;
}

void LRUCacheSimulator::simulate(int cacheSize, const vector<LRUCacheCommand>& commands) {
    LRUCache cache(cacheSize);
    
    cout << "=== LRU КЭШ (емкость: " << cacheSize << ") ===" << endl;
    cout << "----------------------------------------" << endl;
    
    for (size_t i = 0; i < commands.size(); i++) {
        if (commands[i].type == "SET") {
            cout << "SET " << commands[i].key << " = " << commands[i].value << endl;
            cache.set(commands[i].key, commands[i].value);
        } else if (commands[i].type == "GET") {
            string result = cache.get(commands[i].key);
            cout << "GET " << commands[i].key << " -> " << result << endl;
        }
        
        cache.printCache();
        cout << "----------------------------------------" << endl;
    }
}
