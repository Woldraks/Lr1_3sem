#include "HashTable.h"
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

int hashString(const string& key, int capacity) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % capacity;
}

int hashString2(const string& key, int capacity) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 17 + c;
    }
    return 1 + (hash % (capacity - 1));
}

HashTable* CreateOpenAddressingTable(int capacity) {
    HashTable* ht = new HashTable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->collisions = 0;
    ht->probeCount = 0;
    ht->table = new HashTableEntry[capacity];
    
    for (int i = 0; i < capacity; i++) {
        ht->table[i].isOccupied = false;
        ht->table[i].isDeleted = false;
    }
    
    return ht;
}

void DestroyOpenAddressingTable(HashTable* ht) {
    if (ht) {
        delete[] ht->table;
        delete ht;
    }
}

void OpenAddressingInsert(HashTable* ht, string key, string value) {
    if (ht == nullptr) return;
    
    if ((float)ht->size / ht->capacity > 0.7) {
        int oldCapacity = ht->capacity;
        HashTableEntry* oldTable = ht->table;
        
        ht->capacity *= 2;
        ht->size = 0;
        ht->collisions = 0;
        ht->probeCount = 0;
        ht->table = new HashTableEntry[ht->capacity];
        
        for (int i = 0; i < ht->capacity; i++) {
            ht->table[i].isOccupied = false;
            ht->table[i].isDeleted = false;
        }
        
        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
                OpenAddressingInsert(ht, oldTable[i].key, oldTable[i].value);
            }
        }
        
        delete[] oldTable;
    }
    
    int index = hashString(key, ht->capacity);
    int originalIndex = index;
    int probes = 0;
    
    while (ht->table[index].isOccupied && !ht->table[index].isDeleted) {
        if (ht->table[index].key == key) {
            ht->table[index].value = value;
            return;
        }
        
        int step = hashString2(key, ht->capacity);
        index = (index + step) % ht->capacity;
        probes++;
        
        if (index == originalIndex) {
            // Таблица заполнена, нужно расширение
            int oldCapacity = ht->capacity;
            HashTableEntry* oldTable = ht->table;
            
            ht->capacity *= 2;
            ht->size = 0;
            ht->collisions = 0;
            ht->probeCount = 0;
            ht->table = new HashTableEntry[ht->capacity];
            
            for (int i = 0; i < ht->capacity; i++) {
                ht->table[i].isOccupied = false;
                ht->table[i].isDeleted = false;
            }
            
            for (int i = 0; i < oldCapacity; i++) {
                if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
                    OpenAddressingInsert(ht, oldTable[i].key, oldTable[i].value);
                }
            }
            
            delete[] oldTable;
            
            // Пробуем вставить снова
            OpenAddressingInsert(ht, key, value);
            return;
        }
    }
    
    if (probes > 0) {
        ht->collisions++;
        ht->probeCount += probes;
    }
    
    ht->table[index].key = key;
    ht->table[index].value = value;
    ht->table[index].isOccupied = true;
    ht->table[index].isDeleted = false;
    ht->size++;
}

string OpenAddressingGet(HashTable* ht, string key) {
    if (ht == nullptr) return "NOT_FOUND";
    
    int index = hashString(key, ht->capacity);
    int originalIndex = index;
    
    while (ht->table[index].isOccupied) {
        if (!ht->table[index].isDeleted && ht->table[index].key == key) {
            return ht->table[index].value;
        }
        
        int step = hashString2(key, ht->capacity);
        index = (index + step) % ht->capacity;
        
        if (index == originalIndex) break;
    }
    
    return "NOT_FOUND";
}

ChainedHashTable* CreateChainedTable(int capacity) {
    ChainedHashTable* ht = new ChainedHashTable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->collisions = 0;
    ht->buckets = new ChainNode*[capacity];
    
    for (int i = 0; i < capacity; i++) {
        ht->buckets[i] = nullptr;
    }
    
    return ht;
}

void DestroyChainedTable(ChainedHashTable* ht) {
    if (ht) {
        for (int i = 0; i < ht->capacity; i++) {
            ChainNode* current = ht->buckets[i];
            while (current != nullptr) {
                ChainNode* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] ht->buckets;
        delete ht;
    }
}

void ChainedInsert(ChainedHashTable* ht, string key, string value) {
    if (ht == nullptr) return;
    
    int index = hashString(key, ht->capacity);
    
    ChainNode* curr = ht->buckets[index];
    while (curr != nullptr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    
    ChainNode* newNode = new ChainNode;
    newNode->key = key;
    newNode->value = value;
    newNode->next = ht->buckets[index];
    
    if (ht->buckets[index] != nullptr) {
        ht->collisions++;
    }
    
    ht->buckets[index] = newNode;
    ht->size++;
}

string ChainedGet(ChainedHashTable* ht, string key) {
    if (ht == nullptr) return "NOT_FOUND";
    
    int index = hashString(key, ht->capacity);
    
    ChainNode* curr = ht->buckets[index];
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    
    return "NOT_FOUND";
}

void runEmpiricalAnalysis(int numElements, int numSearches) {
    cout << "\n=== ЭМПИРИЧЕСКИЙ АНАЛИЗ ХЕШ-ТАБЛИЦ ===" << endl;
    cout << "Количество элементов: " << numElements << endl;
    cout << "Количество поисков: " << numSearches << endl;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);
    
    // Открытая адресация
    cout << "\nТестирование открытой адресации..." << endl;
    HashTable* openTable = CreateOpenAddressingTable(numElements / 10 + 1);
    
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numElements; i++) {
        string key = "key" + to_string(dis(gen));
        string value = "val" + to_string(i);
        OpenAddressingInsert(openTable, key, value);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto openInsertTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    // Поиск
    start = chrono::high_resolution_clock::now();
    int foundCount = 0;
    for (int i = 0; i < numSearches; i++) {
        string key = "key" + to_string(dis(gen));
        string result = OpenAddressingGet(openTable, key);
        if (result != "NOT_FOUND") foundCount++;
    }
    end = chrono::high_resolution_clock::now();
    auto openSearchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "ОТКРЫТАЯ АДРЕСАЦИЯ:" << endl;
    cout << "  Время вставки: " << openInsertTime << " мкс" << endl;
    cout << "  Время поиска (" << foundCount << " найдено): " << openSearchTime << " мкс" << endl;
    cout << "  Коллизий: " << openTable->collisions << endl;
    if (openTable->size > 0) {
        cout << "  Среднее число проб: " << (openTable->probeCount / (float)openTable->size) << endl;
    }
    
    // Метод цепочек
    cout << "\nТестирование метода цепочек..." << endl;
    ChainedHashTable* chainTable = CreateChainedTable(numElements / 10 + 1);
    
    start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numElements; i++) {
        string key = "key" + to_string(dis(gen));
        string value = "val" + to_string(i);
        ChainedInsert(chainTable, key, value);
    }
    
    end = chrono::high_resolution_clock::now();
    auto chainInsertTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    // Поиск
    start = chrono::high_resolution_clock::now();
    foundCount = 0;
    for (int i = 0; i < numSearches; i++) {
        string key = "key" + to_string(dis(gen));
        string result = ChainedGet(chainTable, key);
        if (result != "NOT_FOUND") foundCount++;
    }
    end = chrono::high_resolution_clock::now();
    auto chainSearchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "МЕТОД ЦЕПОЧЕК:" << endl;
    cout << "  Время вставки: " << chainInsertTime << " мкс" << endl;
    cout << "  Время поиска (" << foundCount << " найдено): " << chainSearchTime << " мкс" << endl;
    cout << "  Коллизий: " << chainTable->collisions << endl;
    
    // Очистка памяти
    DestroyOpenAddressingTable(openTable);
    DestroyChainedTable(chainTable);
}