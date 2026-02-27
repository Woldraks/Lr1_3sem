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
    cout << "Проверка правил из задания:" << endl;
    cout << "Правило 1: Пустая строка - корректный XML" << endl;
    cout << "Правило 2: Если A и B корректны, то AB корректен" << endl;
    cout << "Правило 3: Если A корректен, то <X>A</X> корректен" << endl;
    cout << "----------------------------------------" << endl;
    
    vector<string> testCases;
    testCases.push_back("<a></b>");           // нужно восстановить
    testCases.push_back("<a><b></a></b>");    // нужно восстановить
    testCases.push_back("<a><b></b></a>");    // уже корректный
    testCases.push_back("");                   // пустая строка (правило 1)
    testCases.push_back("<a></a><b></b>");    // конкатенация (правило 2)
    
    for (size_t i = 0; i < testCases.size(); i++) {
        cout << "Исходная строка: \"" << testCases[i] << "\"" << endl;
        
        // Проверяем, корректна ли строка
        bool isValid = XMLValidator::isValidXML(testCases[i]);
        cout << "Корректна? " << (isValid ? "да" : "нет") << endl;
        
        if (!isValid) {
            string recovered = XMLValidator::recoverXML(testCases[i]);
            if (recovered.empty()) {
                cout << "Восстановленная: (невозможно восстановлить)" << endl;
            } else {
                cout << "Восстановленная: \"" << recovered << "\"" << endl;
                cout << "После восстановления корректна? " 
                     << (XMLValidator::isValidXML(recovered) ? "да" : "нет") << endl;
            }
        }
        cout << "-------------------" << endl;
    }
}

void testHashSet() {
    cout << "\n=== ТЕСТ 2: АТД Множество (операции над множествами) ===" << endl;
    cout << "Команды: SETADD, SETDEL, SET_AT" << endl;
    cout << "----------------------------------------" << endl;
    
    HashSet* set = CreateHashSet(8);
    
    cout << "SETADD apple" << endl;
    SetAdd(set, "apple");
    
    cout << "SETADD banana" << endl;
    SetAdd(set, "banana");
    
    cout << "SETADD orange" << endl;
    SetAdd(set, "orange");
    
    cout << "SET_AT apple: " << (SetAt(set, "apple") ? "true" : "false") << endl;
    cout << "SET_AT grape: " << (SetAt(set, "grape") ? "true" : "false") << endl;
    
    cout << "SETDEL banana" << endl;
    SetDel(set, "banana");
    
    cout << "SET_AT banana: " << (SetAt(set, "banana") ? "true" : "false") << endl;
    
    cout << "Финальное множество: ";
    SetPrint(set);
    
    DestroyHashSet(set);
}

void testSetPartition() {
    cout << "\n=== ТЕСТ 3.1: Разбиение множества (использует HashSet из задания 2) ===" << endl;
    
    // Тест 1: Разбиение на равные суммы
    Array* set = CreateArray(8);
    Push(set, "4");
    Push(set, "10");
    Push(set, "5");
    Push(set, "1");
    Push(set, "3");
    Push(set, "7");
    
    int targetSum = 15;
    
    cout << "Исходное множество: ";
    Print(set);
    cout << "Целевая сумма: " << targetSum << endl;
    
    PartitionResult result = SetPartitioner::partitionIntoEqualSums(set, targetSum);
    SetPartitioner::printPartition(result);
    SetPartitioner::destroyPartition(result);
    
    // Тест 2: Поиск максимального пересечения (вариант 4)
    cout << "\nПоиск двух множеств с максимальным пересечением:" << endl;
    
    Array* sets[4];
    sets[0] = CreateArray(8);
    Push(sets[0], "1"); Push(sets[0], "2"); Push(sets[0], "3");
    
    sets[1] = CreateArray(8);
    Push(sets[1], "2"); Push(sets[1], "3"); Push(sets[1], "4");
    
    sets[2] = CreateArray(8);
    Push(sets[2], "5"); Push(sets[2], "6");
    
    sets[3] = CreateArray(8);
    Push(sets[3], "3"); Push(sets[3], "4"); Push(sets[3], "5");
    
    for (int i = 0; i < 4; i++) {
        cout << "Множество " << i + 1 << ": ";
        Print(sets[i]);
    }
    
    SetPartitioner::findMaxIntersection(sets, 4);
    
    for (int i = 0; i < 4; i++) {
        Destroy(sets[i]);
    }
    Destroy(set);
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
    IsomorphicChecker::printResult("abca", "zbxz");
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
    cout << "===========================================" << endl;
    cout << "     ЛАБОРАТОРНАЯ РАБОТА №2" << endl;
    cout << "Максимальное использование структур из 1 лабораторной" << endl;
    cout << "===========================================" << endl;
    
    testXML();
    testHashSet();
    testSetPartition();
    testSubarrays();
    testBSTHeight();
    testHashTable();
    testIsomorphicStrings();
    testLRUCache();
    
    cout << "\n===========================================" << endl;
    cout << "           ВСЕ ТЕСТЫ ВЫПОЛНЕНЫ" << endl;
    cout << "===========================================" << endl;
    
    return 0;
}
