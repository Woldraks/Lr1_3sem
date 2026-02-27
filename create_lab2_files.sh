#!/bin/bash

echo "Создание новых файлов для лабораторной работы №2..."

# Создаем новые директории
mkdir -p HashSet
mkdir -p SetPartition
mkdir -p Array/Subarrays
mkdir -p Tree/BSTHeight
mkdir -p HashTable
mkdir -p LRUCache

# ========== ЗАДАНИЕ 1.7 - XML (Stack) ==========
cat > Stack/XMLValidator.h << 'EOF'
#pragma once
#include <string>
using namespace std;

class XMLValidator {
public:
    static string recoverXML(const string& corrupted);
    static bool isValidXML(const string& xml);
};
EOF

cat > Stack/XMLValidator.cpp << 'EOF'
#include "XMLValidator.h"
#include "Stack.h"
#include <iostream>
using namespace std;

string XMLValidator::recoverXML(const string& corrupted) {
    if (corrupted.length() < 7 || corrupted.length() > 1000) {
        return "";
    }
    
    string alphabet = "abcdefghijklmnopqrstuvwxyz<>/";
    
    for (size_t i = 0; i < corrupted.length(); i++) {
        char original = corrupted[i];
        
        for (char c : alphabet) {
            if (c == original) continue;
            
            string candidate = corrupted;
            candidate[i] = c;
            
            if (isValidXML(candidate)) {
                return candidate;
            }
        }
    }
    
    return "";
}

bool XMLValidator::isValidXML(const string& xml) {
    Stack* stack = CreateStack();
    string currentTag = "";
    bool isOpening = false;
    bool isClosing = false;
    
    for (char c : xml) {
        if (c == '<') {
            if (!currentTag.empty() && !isOpening && !isClosing) {
                ClearStack(stack);
                delete stack;
                return false;
            }
            currentTag = "";
            isOpening = true;
        }
        else if (c == '>') {
            if (currentTag.empty()) {
                ClearStack(stack);
                delete stack;
                return false;
            }
            
            if (isClosing) {
                if (isEmpty(stack)) {
                    ClearStack(stack);
                    delete stack;
                    return false;
                }
                
                string lastTag = Pop(stack);
                if (lastTag != currentTag) {
                    ClearStack(stack);
                    delete stack;
                    return false;
                }
            }
            else if (isOpening) {
                Push(stack, CreateStackElement(currentTag));
            }
            
            currentTag = "";
            isOpening = false;
            isClosing = false;
        }
        else if (c == '/') {
            if (isOpening && currentTag.empty()) {
                isOpening = false;
                isClosing = true;
            }
            else {
                ClearStack(stack);
                delete stack;
                return false;
            }
        }
        else if (isalpha(c)) {
            if (isOpening || isClosing) {
                currentTag += c;
            }
            else {
                ClearStack(stack);
                delete stack;
                return false;
            }
        }
        else {
            ClearStack(stack);
            delete stack;
            return false;
        }
    }
    
    bool result = isEmpty(stack);
    ClearStack(stack);
    delete stack;
    return result;
}
EOF

# ========== ЗАДАНИЕ 2 - HashSet ==========
cat > HashSet/HashSet.h << 'EOF'
#pragma once
#include <string>
using namespace std;

struct HashSetNode {
    string key;
    bool isOccupied;
    bool isDeleted;
};

struct HashSet {
    HashSetNode* table;
    int capacity;
    int size;
    int collisions;
};

HashSet* CreateHashSet(int initialCapacity);
void SetAdd(HashSet* set, string key);
void SetDel(HashSet* set, string key);
bool SetAt(HashSet* set, string key);
int hashFunction(const string& key, int capacity);
void resizeHashSet(HashSet* set);
void DestroyHashSet(HashSet* set);
EOF

cat > HashSet/HashSet.cpp << 'EOF'
#include "HashSet.h"
#include <iostream>
using namespace std;

HashSet* CreateHashSet(int initialCapacity) {
    HashSet* set = new HashSet;
    set->capacity = initialCapacity;
    set->size = 0;
    set->collisions = 0;
    set->table = new HashSetNode[initialCapacity];
    
    for (int i = 0; i < initialCapacity; i++) {
        set->table[i].isOccupied = false;
        set->table[i].isDeleted = false;
    }
    
    return set;
}

int hashFunction(const string& key, int capacity) {
    unsigned long hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % capacity;
}

void resizeHashSet(HashSet* set) {
    int oldCapacity = set->capacity;
    HashSetNode* oldTable = set->table;
    
    set->capacity *= 2;
    set->size = 0;
    set->collisions = 0;
    set->table = new HashSetNode[set->capacity];
    
    for (int i = 0; i < set->capacity; i++) {
        set->table[i].isOccupied = false;
        set->table[i].isDeleted = false;
    }
    
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].isOccupied && !oldTable[i].isDeleted) {
            SetAdd(set, oldTable[i].key);
        }
    }
    
    delete[] oldTable;
}

void SetAdd(HashSet* set, string key) {
    if ((float)set->size / set->capacity > 0.75) {
        resizeHashSet(set);
    }
    
    int index = hashFunction(key, set->capacity);
    int originalIndex = index;
    bool collided = false;
    
    while (set->table[index].isOccupied && !set->table[index].isDeleted) {
        if (set->table[index].key == key) {
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
    
    set->table[index].key = key;
    set->table[index].isOccupied = true;
    set->table[index].isDeleted = false;
    set->size++;
}

void SetDel(HashSet* set, string key) {
    int index = hashFunction(key, set->capacity);
    int originalIndex = index;
    
    while (set->table[index].isOccupied) {
        if (!set->table[index].isDeleted && set->table[index].key == key) {
            set->table[index].isDeleted = true;
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
    
    while (set->table[index].isOccupied) {
        if (!set->table[index].isDeleted && set->table[index].key == key) {
            return true;
        }
        
        index = (index + 1) % set->capacity;
        if (index == originalIndex) break;
    }
    
    return false;
}

void DestroyHashSet(HashSet* set) {
    delete[] set->table;
    delete set;
}
EOF

# ========== ЗАДАНИЕ 3.1 - SetPartition ==========
cat > SetPartition/SetPartition.h << 'EOF'
#pragma once
#include <vector>
#include <string>
using namespace std;

struct PartitionResult {
    vector<vector<int>> subsets;
    bool possible;
    int targetSum;
};

class SetPartitioner {
public:
    static PartitionResult partitionIntoEqualSums(const vector<int>& set, int targetSum);
    static void printPartition(const PartitionResult& result);
    
private:
    static bool canPartition(const vector<int>& set, int targetSum, 
                             vector<bool>& used, vector<int>& current, 
                             vector<vector<int>>& result, int currentSum);
};
EOF

cat > SetPartition/SetPartition.cpp << 'EOF'
#include "SetPartition.h"
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;

PartitionResult SetPartitioner::partitionIntoEqualSums(const vector<int>& set, int targetSum) {
    PartitionResult result;
    result.possible = false;
    result.targetSum = targetSum;
    
    int totalSum = accumulate(set.begin(), set.end(), 0);
    
    if (totalSum % targetSum != 0) {
        return result;
    }
    
    vector<bool> used(set.size(), false);
    vector<int> current;
    
    vector<int> sortedSet = set;
    sort(sortedSet.begin(), sortedSet.end(), greater<int>());
    
    if (canPartition(sortedSet, targetSum, used, current, result.subsets, 0)) {
        result.possible = true;
    }
    
    return result;
}

bool SetPartitioner::canPartition(const vector<int>& set, int targetSum,
                                  vector<bool>& used, vector<int>& current,
                                  vector<vector<int>>& result, int currentSum) {
    if (currentSum == targetSum) {
        result.push_back(current);
        current.clear();
        return true;
    }
    
    for (size_t i = 0; i < set.size(); i++) {
        if (!used[i] && currentSum + set[i] <= targetSum) {
            used[i] = true;
            current.push_back(set[i]);
            
            if (canPartition(set, targetSum, used, current, result, currentSum + set[i])) {
                return true;
            }
            
            current.pop_back();
            used[i] = false;
            
            while (i + 1 < set.size() && set[i] == set[i + 1]) {
                i++;
            }
        }
    }
    
    return false;
}

void SetPartitioner::printPartition(const PartitionResult& result) {
    if (!result.possible) {
        cout << "Невозможно разбить множество на подмножества с суммой " 
             << result.targetSum << endl;
        return;
    }
    
    cout << "Разбиение на подмножества с суммой " << result.targetSum << ":" << endl;
    for (size_t i = 0; i < result.subsets.size(); i++) {
        cout << "Подмножество " << i + 1 << ": {";
        for (size_t j = 0; j < result.subsets[i].size(); j++) {
            cout << result.subsets[i][j];
            if (j < result.subsets[i].size() - 1) cout << ", ";
        }
        cout << "}" << endl;
    }
}
EOF

# ========== ЗАДАНИЕ 4.1 - Subarrays ==========
cat > Array/Subarrays/Subarrays.h << 'EOF'
#pragma once
#include <vector>
#include <string>
using namespace std;

class SubarrayGenerator {
public:
    static vector<vector<string>> generateAllSubarrays(const vector<string>& arr);
    static void printAllSubarrays(const vector<string>& arr);
};
EOF

cat > Array/Subarrays/Subarrays.cpp << 'EOF'
#include "Subarrays.h"
#include <iostream>
using namespace std;

vector<vector<string>> SubarrayGenerator::generateAllSubarrays(const vector<string>& arr) {
    vector<vector<string>> result;
    int n = arr.size();
    
    // Добавляем пустой подмассив
    result.push_back(vector<string>());
    
    // Генерируем все возможные подмассивы
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            vector<string> subarray;
            for (int k = i; k <= j; k++) {
                subarray.push_back(arr[k]);
            }
            result.push_back(subarray);
        }
    }
    
    return result;
}

void SubarrayGenerator::printAllSubarrays(const vector<string>& arr) {
    vector<vector<string>> subarrays = generateAllSubarrays(arr);
    
    cout << "Все различные подмассивы:" << endl;
    cout << "[";
    for (size_t i = 0; i < subarrays.size(); i++) {
        cout << "{";
        for (size_t j = 0; j < subarrays[i].size(); j++) {
            cout << subarrays[i][j];
            if (j < subarrays[i].size() - 1) cout << ", ";
        }
        cout << "}";
        if (i < subarrays.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    cout << "Всего подмассивов: " << subarrays.size() << endl;
}
EOF

# ========== ЗАДАНИЕ 5.7 - BSTHeight ==========
cat > Tree/BSTHeight/BSTHeight.h << 'EOF'
#pragma once
#include "../Tree.h"
#include <vector>
using namespace std;

class BSTHeightCalculator {
public:
    static int calculateHeight(BST* tree);
    static int calculateHeightRec(BSTNode* node);
    static void processSequence(BST* tree, const vector<int>& sequence);
};
EOF

cat > Tree/BSTHeight/BSTHeight.cpp << 'EOF'
#include "BSTHeight.h"
#include <iostream>
#include <algorithm>
using namespace std;

int BSTHeightCalculator::calculateHeightRec(BSTNode* node) {
    if (node == nullptr) {
        return 0;
    }
    
    int leftHeight = calculateHeightRec(node->left);
    int rightHeight = calculateHeightRec(node->right);
    
    return 1 + max(leftHeight, rightHeight);
}

int BSTHeightCalculator::calculateHeight(BST* tree) {
    if (tree == nullptr || tree->root == nullptr) {
        return 0;
    }
    
    return calculateHeightRec(tree->root);
}

void BSTHeightCalculator::processSequence(BST* tree, const vector<int>& sequence) {
    cout << "Обработка последовательности: ";
    for (int val : sequence) {
        if (val == 0) break;
        cout << val << " ";
        InsertNode(tree, val);
    }
    cout << endl;
    
    int height = calculateHeight(tree);
    cout << "Высота построенного дерева: " << height << endl;
}
EOF

# ========== ЗАДАНИЕ 6.1 - HashTable ==========
cat > HashTable/HashTable.h << 'EOF'
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
EOF

cat > HashTable/HashTable.cpp << 'EOF'
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

void OpenAddressingInsert(HashTable* ht, string key, string value) {
    if ((float)ht->size / ht->capacity > 0.7) {
        int oldCapacity = ht->capacity;
        HashTableEntry* oldTable = ht->table;
        
        ht->capacity *= 2;
        ht->size = 0;
        ht->collisions = 0;
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

void ChainedInsert(ChainedHashTable* ht, string key, string value) {
    int index = hashString(key, ht->capacity);
    
    ChainNode* curr = ht->buckets[index];
    while (curr != nullptr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    
    ChainNode* newNode = new ChainNode{key, value, ht->buckets[index]};
    
    if (ht->buckets[index] != nullptr) {
        ht->collisions++;
    }
    
    ht->buckets[index] = newNode;
    ht->size++;
}

string ChainedGet(ChainedHashTable* ht, string key) {
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
    
    HashTable* openTable = CreateOpenAddressingTable(numElements / 10);
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numElements; i++) {
        string key = "key" + to_string(dis(gen));
        string value = "val" + to_string(i);
        OpenAddressingInsert(openTable, key, value);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto openInsertTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numSearches; i++) {
        string key = "key" + to_string(dis(gen));
        OpenAddressingGet(openTable, key);
    }
    end = chrono::high_resolution_clock::now();
    auto openSearchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "\nОТКРЫТАЯ АДРЕСАЦИЯ:" << endl;
    cout << "  Время вставки: " << openInsertTime << " мкс" << endl;
    cout << "  Время поиска: " << openSearchTime << " мкс" << endl;
    cout << "  Коллизий: " << openTable->collisions << endl;
    cout << "  Среднее число проб: " << (openTable->probeCount / (float)openTable->size) << endl;
    
    ChainedHashTable* chainTable = CreateChainedTable(numElements / 10);
    start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numElements; i++) {
        string key = "key" + to_string(dis(gen));
        string value = "val" + to_string(i);
        ChainedInsert(chainTable, key, value);
    }
    
    end = chrono::high_resolution_clock::now();
    auto chainInsertTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numSearches; i++) {
        string key = "key" + to_string(dis(gen));
        ChainedGet(chainTable, key);
    }
    end = chrono::high_resolution_clock::now();
    auto chainSearchTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "\nМЕТОД ЦЕПОЧЕК:" << endl;
    cout << "  Время вставки: " << chainInsertTime << " мкс" << endl;
    cout << "  Время поиска: " << chainSearchTime << " мкс" << endl;
    cout << "  Коллизий: " << chainTable->collisions << endl;
}
EOF

# ========== ЗАДАНИЕ 6.7 - IsomorphicStrings ==========
cat > HashTable/IsomorphicStrings.h << 'EOF'
#pragma once
#include <string>
using namespace std;

class IsomorphicChecker {
public:
    static bool areIsomorphic(const string& s1, const string& s2);
    static void printResult(const string& s1, const string& s2);
};
EOF

cat > HashTable/IsomorphicStrings.cpp << 'EOF'
#include "IsomorphicStrings.h"
#include <iostream>
#include <unordered_map>
using namespace std;

bool IsomorphicChecker::areIsomorphic(const string& s1, const string& s2) {
    if (s1.length() != s2.length()) {
        return false;
    }
    
    unordered_map<char, char> map1;
    unordered_map<char, char> map2;
    
    for (size_t i = 0; i < s1.length(); i++) {
        char c1 = s1[i];
        char c2 = s2[i];
        
        if (map1.find(c1) != map1.end()) {
            if (map1[c1] != c2) {
                return false;
            }
        } else {
            map1[c1] = c2;
        }
        
        if (map2.find(c2) != map2.end()) {
            if (map2[c2] != c1) {
                return false;
            }
        } else {
            map2[c2] = c1;
        }
    }
    
    return true;
}

void IsomorphicChecker::printResult(const string& s1, const string& s2) {
    cout << "Строки \"" << s1 << "\" и \"" << s2 << "\" ";
    if (areIsomorphic(s1, s2)) {
        cout << "ИЗОМОРФНЫ" << endl;
    } else {
        cout << "НЕ ИЗОМОРФНЫ" << endl;
    }
}
EOF

# ========== ЗАДАНИЕ 7.1 - LRUCache ==========
cat > LRUCache/LRUCache.h << 'EOF'
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
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
    unordered_map<string, LRUNode*> cache;
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
    string type;
    string key;
    string value;
};

class LRUCacheSimulator {
public:
    static void simulate(int cacheSize, const vector<LRUCacheCommand>& commands);
};
EOF

cat > LRUCache/LRUCache.cpp << 'EOF'
#include "LRUCache.h"
#include <iostream>
using namespace std;

LRUCache::LRUCache(int cap) : capacity(cap), head(nullptr), tail(nullptr) {}

LRUCache::~LRUCache() {
    LRUNode* current = head;
    while (current != nullptr) {
        LRUNode* next = current->next;
        delete current;
        current = next;
    }
}

void LRUCache::moveToHead(LRUNode* node) {
    if (node == head) return;
    
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == tail) {
        tail = node->prev;
    }
    
    node->next = head;
    node->prev = nullptr;
    
    if (head) {
        head->prev = node;
    }
    head = node;
    
    if (tail == nullptr) {
        tail = node;
    }
}

void LRUCache::removeTail() {
    if (tail == nullptr) return;
    
    cache.erase(tail->key);
    
    LRUNode* toDelete = tail;
    tail = tail->prev;
    
    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    
    delete toDelete;
}

string LRUCache::get(string key) {
    auto it = cache.find(key);
    if (it == cache.end()) {
        return "-1";
    }
    
    LRUNode* node = it->second;
    moveToHead(node);
    
    return node->value;
}

void LRUCache::set(string key, string value) {
    auto it = cache.find(key);
    
    if (it != cache.end()) {
        LRUNode* node = it->second;
        node->value = value;
        moveToHead(node);
    } else {
        LRUNode* newNode = new LRUNode(key, value);
        cache[key] = newNode;
        
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        
        if ((int)cache.size() > capacity) {
            removeTail();
        }
    }
}

void LRUCache::printCache() {
    cout << "LRU Cache (от наиболее свежего к наименее):" << endl;
    LRUNode* current = head;
    while (current != nullptr) {
        cout << "  " << current->key << " -> " << current->value << endl;
        current = current->next;
    }
    cout << "Размер: " << cache.size() << "/" << capacity << endl;
}

void LRUCacheSimulator::simulate(int cacheSize, const vector<LRUCacheCommand>& commands) {
    LRUCache cache(cacheSize);
    
    cout << "=== LRU КЭШ (емкость: " << cacheSize << ") ===" << endl;
    
    for (const auto& cmd : commands) {
        if (cmd.type == "SET") {
            cout << "SET " << cmd.key << " = " << cmd.value << endl;
            cache.set(cmd.key, cmd.value);
        } else if (cmd.type == "GET") {
            string result = cache.get(cmd.key);
            cout << "GET " << cmd.key << " -> " << result << endl;
        }
        
        cache.printCache();
        cout << "-------------------" << endl;
    }
}
EOF

# ========== MAIN.CPP для тестирования ==========
cat > main_lab2.cpp << 'EOF'
#include <iostream>
#include <vector>
#include <string>
#include "Stack/XMLValidator.h"
#include "HashSet/HashSet.h"
#include "SetPartition/SetPartition.h"
#include "Array/Subarrays/Subarrays.h"
#include "Tree/BSTHeight/BSTHeight.h"
#include "HashTable/HashTable.h"
#include "HashTable/IsomorphicStrings.h"
#include "LRUCache/LRUCache.h"
#include "Tree/Tree.h"

using namespace std;

void testXML() {
    cout << "\n=== ТЕСТ 1.7: XML Восстановление ===" << endl;
    vector<string> testCases = {
        "<a></b>",
        "<a><b></a></b>",
        "<a><b></b></a>"
    };
    
    for (const auto& test : testCases) {
        cout << "Исходная строка: " << test << endl;
        string recovered = XMLValidator::recoverXML(test);
        cout << "Восстановленная: " << recovered << endl;
        cout << "-------------------" << endl;
    }
}

void testHashSet() {
    cout << "\n=== ТЕСТ 2: АТД Множество ===" << endl;
    HashSet* set = CreateHashSet(8);
    
    SetAdd(set, "apple");
    SetAdd(set, "banana");
    SetAdd(set, "orange");
    
    cout << "SetAt(apple): " << (SetAt(set, "apple") ? "true" : "false") << endl;
    cout << "SetAt(grape): " << (SetAt(set, "grape") ? "true" : "false") << endl;
    
    SetDel(set, "banana");
    cout << "После удаления banana:" << endl;
    cout << "SetAt(banana): " << (SetAt(set, "banana") ? "true" : "false") << endl;
    
    DestroyHashSet(set);
}

void testSetPartition() {
    cout << "\n=== ТЕСТ 3.1: Разбиение множества ===" << endl;
    vector<int> set = {4, 10, 5, 1, 3, 7};
    int targetSum = 15;
    
    PartitionResult result = SetPartitioner::partitionIntoEqualSums(set, targetSum);
    SetPartitioner::printPartition(result);
}

void testSubarrays() {
    cout << "\n=== ТЕСТ 4.1: Различные подмассивы ===" << endl;
    vector<string> arr = {"x", "y", "z"};
    SubarrayGenerator::printAllSubarrays(arr);
}

void testBSTHeight() {
    cout << "\n=== ТЕСТ 5.7: Высота дерева ===" << endl;
    BST* tree = CreateBST();
    vector<int> sequence = {7, 3, 2, 1, 9, 5, 4, 6, 8, 0};
    
    BSTHeightCalculator::processSequence(tree, sequence);
    Destroy(tree);
}

void testHashTable() {
    cout << "\n=== ТЕСТ 6.1: Хеш-таблицы ===" << endl;
    runEmpiricalAnalysis(1000, 100);
}

void testIsomorphicStrings() {
    cout << "\n=== ТЕСТ 6.7: Изоморфные строки ===" << endl;
    IsomorphicChecker::printResult("fall", "redd");
    IsomorphicChecker::printResult("mad", "odd");
    IsomorphicChecker::printResult("paper", "title");
}

void testLRUCache() {
    cout << "\n=== ТЕСТ 7.1: LRU кэш ===" << endl;
    vector<LRUCacheCommand> commands = {
        {"SET", "1", "2"},
        {"GET", "1", ""},
        {"SET", "2", "3"},
        {"SET", "1", "5"},
        {"SET", "4", "5"},
        {"SET", "6", "7"},
        {"GET", "4", ""},
        {"SET", "1", "2"},
        {"GET", "3", ""}
    };
    
    LRUCacheSimulator::simulate(2, commands);
}

int main() {
    cout << "===== ЛАБОРАТОРНАЯ РАБОТА №2 =====" << endl;
    
    testXML();
    testHashSet();
    testSetPartition();
    testSubarrays();
    testBSTHeight();
    testHashTable();
    testIsomorphicStrings();
    testLRUCache();
    
    return 0;
}
EOF

# ========== СКРИПТ ДЛЯ СБОРКИ ==========
cat > build_lab2.sh << 'EOF'
#!/bin/bash

echo "Компиляция лабораторной работы №2..."

g++ -std=c++11 \
    Array/Array.cpp \
    ForwardList/SinglyLinkedList.cpp \
    LinkedList/DoublyLinkedList.cpp \
    Queue/Queue.cpp \
    Stack/Stack.cpp \
    Tree/Tree.cpp \
    Stack/XMLValidator.cpp \
    HashSet/HashSet.cpp \
    SetPartition/SetPartition.cpp \
    Array/Subarrays/Subarrays.cpp \
    Tree/BSTHeight/BSTHeight.cpp \
    HashTable/HashTable.cpp \
    HashTable/IsomorphicStrings.cpp \
    LRUCache/LRUCache.cpp \
    main_lab2.cpp \
    -o lab2

if [ $? -eq 0 ]; then
    echo "Компиляция успешна! Запуск программы..."
    ./lab2
else
    echo "Ошибка компиляции!"
fi
EOF

chmod +x build_lab2.sh

echo "Все новые файлы успешно созданы!"
echo "Для компиляции и запуска выполните: ./build_lab2.sh"
echo ""
echo "ВАЖНО: Убедитесь, что файлы из первой лабораторной работы находятся в текущей директории:"
echo "  - Array/Array.cpp, Array/Array.h"
echo "  - ForwardList/SinglyLinkedList.cpp, ForwardList/SinglyLinkedList.h"
echo "  - LinkedList/DoublyLinkedList.cpp, LinkedList/DoublyLinkedList.h"
echo "  - Queue/Queue.cpp, Queue/Queue.h"
echo "  - Stack/Stack.cpp, Stack/Stack.h"
echo "  - Tree/Tree.cpp, Tree/Tree.h"
echo "  - PositionEnum.h"
