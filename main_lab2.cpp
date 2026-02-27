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
#include "Array/Array.h"

using namespace std;

void testXML() {
    cout << "\n=== ТЕСТ 1.7: XML Восстановление (использует Stack из 1 лабы) ===" << endl;
    vector<string> testCases;
    testCases.push_back("<a></b>");
    testCases.push_back("<a><b></a></b>");
    testCases.push_back("<a><b></b></a>");
    testCases.push_back("a");
    
    for (size_t i = 0; i < testCases.size(); i++) {
        cout << "Исходная строка: " << testCases[i] << endl;
        string recovered = XMLValidator::recoverXML(testCases[i]);
        if (recovered.empty()) {
            cout << "Восстановленная: (невозможно восстановить)" << endl;
        } else {
            cout << "Восстановленная: " << recovered << endl;
        }
        cout << "-------------------" << endl;
    }
}

void testHashSet() {
    cout << "\n=== ТЕСТ 2: АТД Множество (операции над множествами) ===" << endl;
    
    HashSet* set1 = CreateHashSet(8);
    SetAdd(set1, "apple");
    SetAdd(set1, "banana");
    SetAdd(set1, "orange");
    SetAdd(set1, "grape");
    
    HashSet* set2 = CreateHashSet(8);
    SetAdd(set2, "banana");
    SetAdd(set2, "orange");
    SetAdd(set2, "kiwi");
    SetAdd(set2, "mango");
    
    cout << "Множество 1: ";
    SetPrint(set1);
    cout << "Множество 2: ";
    SetPrint(set2);
    
    HashSet* unionSet = SetUnion(set1, set2);
    cout << "Объединение: ";
    SetPrint(unionSet);
    
    HashSet* intersectionSet = SetIntersection(set1, set2);
    cout << "Пересечение: ";
    SetPrint(intersectionSet);
    
    HashSet* diffSet = SetDifference(set1, set2);
    cout << "Разность (set1 \\ set2): ";
    SetPrint(diffSet);
    
    DestroyHashSet(set1);
    DestroyHashSet(set2);
    DestroyHashSet(unionSet);
    DestroyHashSet(intersectionSet);
    DestroyHashSet(diffSet);
}



void testSubarrays() {
    cout << "\n=== ТЕСТ 4.1: Различные подмассивы (использует Array из 1 лабы) ===" << endl;
    Array* arr = CreateArray(8);
    Push(arr, "x");
    Push(arr, "y");
    Push(arr, "z");
    
    cout << "Исходный массив: ";
    Print(arr);
    
    SubarrayGenerator::printAllSubarrays(arr);
    Destroy(arr);
}

void testBSTHeight() {
    cout << "\n=== ТЕСТ 5.7: Высота дерева (использует Tree из 1 лабы) ===" << endl;
    BST* tree = CreateBST();
    int sequence[] = {7, 3, 2, 1, 9, 5, 4, 6, 8, 0};
    int seqSize = 10;
    
    cout << "Обработка последовательности: ";
    for (int i = 0; i < seqSize && sequence[i] != 0; i++) {
        cout << sequence[i] << " ";
        InsertNode(tree, sequence[i]);
    }
    cout << endl;
    
    int height = BSTHeightCalculator::calculateHeight(tree);
    cout << "Высота построенного дерева: " << height << endl;
    
    Destroy(tree);
}

void testHashTable() {
    cout << "\n=== ТЕСТ 6.1: Хеш-таблицы (использует ForwardList из 1 лабы) ===" << endl;
    runEmpiricalAnalysis(1000, 100);
}

void testIsomorphicStrings() {
    cout << "\n=== ТЕСТ 6.7: Изоморфные строки (алгоритмическое) ===" << endl;
    IsomorphicChecker::printResult("fall", "redd");
    IsomorphicChecker::printResult("mad", "odd");
    IsomorphicChecker::printResult("paper", "title");
}

void testLRUCache() {
    cout << "\n=== ТЕСТ 7.1: LRU кэш (использует DoublyLinkedList из 1 лабы) ===" << endl;
    
    vector<LRUCacheCommand> commands;
    
    LRUCacheCommand cmd1; cmd1.type = "SET"; cmd1.key = "1"; cmd1.value = "2"; commands.push_back(cmd1);
    LRUCacheCommand cmd2; cmd2.type = "GET"; cmd2.key = "1"; cmd2.value = ""; commands.push_back(cmd2);
    LRUCacheCommand cmd3; cmd3.type = "SET"; cmd3.key = "2"; cmd3.value = "3"; commands.push_back(cmd3);
    LRUCacheCommand cmd4; cmd4.type = "SET"; cmd4.key = "1"; cmd4.value = "5"; commands.push_back(cmd4);
    LRUCacheCommand cmd5; cmd5.type = "SET"; cmd5.key = "4"; cmd5.value = "5"; commands.push_back(cmd5);
    LRUCacheCommand cmd6; cmd6.type = "SET"; cmd6.key = "6"; cmd6.value = "7"; commands.push_back(cmd6);
    LRUCacheCommand cmd7; cmd7.type = "GET"; cmd7.key = "4"; cmd7.value = ""; commands.push_back(cmd7);
    LRUCacheCommand cmd8; cmd8.type = "SET"; cmd8.key = "1"; cmd8.value = "2"; commands.push_back(cmd8);
    LRUCacheCommand cmd9; cmd9.type = "GET"; cmd9.key = "3"; cmd9.value = ""; commands.push_back(cmd9);
    
    LRUCacheSimulator::simulate(2, commands);
}

int main() {
    cout << "===== ЛАБОРАТОРНАЯ РАБОТА №2 =====" << endl;
    cout << "Максимальное использование структур из 1 лабораторной" << endl;
    cout << "===========================================" << endl;
    
    testXML();
    testHashSet();
    
    testSubarrays();
    testBSTHeight();
    testHashTable();
    testIsomorphicStrings();
    testLRUCache();
    
    return 0;
}
