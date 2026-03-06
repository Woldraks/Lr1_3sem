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
    cout << "\n=== ТЕСТ 1.7: XML Восстановление " << endl;
    cout << "Проверка правил из задания:" << endl;
    cout << "Правило 1: Пустая строка - корректный XML" << endl;
    cout << "Правило 2: Если A и B корректны, то AB корректен" << endl;
    cout << "Правило 3: Если A корректен, то <X>A</X> корректен" << endl;
    cout << "----------------------------------------" << endl;
    
    vector<string> testCases;
    testCases.push_back("<a></b>");         
    testCases.push_back("<a><b></a></b>");   
    testCases.push_back("<a><b></b></a>");    
    testCases.push_back("");                   
    testCases.push_back("<a></a><b></b>");    
    testCases.push_back("<x><x><x><x>"); 
    testCases.push_back("<a><b><<b></a>");

     
    
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
    cout << "\n=== ТЕСТ 2: АТД Множество  ===" << endl;
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
    cout << "\n=== ТЕСТ 3.1: Разбиение множества ===" << endl;
  
    Array* set = CreateArray(8);
    Push(set, "4");
    Push(set, "10");
    Push(set, "5");
    Push(set, "1");
    Push(set, "3");
    Push(set, "7");
    
    cout << "Исходное множество: ";
    Print(set);
    
    // Тест 1: Сумма 15
    cout << "\n Тест 1: Целевая сумма = 15" << endl;
    PartitionResult result1 = SetPartitioner::partitionIntoEqualSums(set, 15);
    SetPartitioner::printPartition(result1);
    SetPartitioner::destroyPartition(result1);
    
    // Тест 2: Сумма 10
    cout << "\n Тест 2: Целевая сумма = 10" << endl;
    PartitionResult result2 = SetPartitioner::partitionIntoEqualSums(set, 10);
    SetPartitioner::printPartition(result2);
    SetPartitioner::destroyPartition(result2);
    
    // Тест 3: Сумма 7 
    cout << "\n Тест 3: Целевая сумма = 7" << endl;
    PartitionResult result3 = SetPartitioner::partitionIntoEqualSums(set, 7);
    SetPartitioner::printPartition(result3);
    SetPartitioner::destroyPartition(result3);
    
    Destroy(set);
}

void testSubarrays() {
    cout << "\n=== ТЕСТ 4.1: Различные подмассивы  ===" << endl;
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
    cout << "\n=== ТЕСТ 5.7: Высота дерева ===" << endl;
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
    cout << "\n=== ТЕСТ 6.1: Хеш-таблицы  ===" << endl;
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
    cout << "\n=== ТЕСТ 7.1: LRU кэш ===" << endl;
    
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
    //LRUCacheCommand cmd10; cmd10.type = "SET"; cmd10.key = "8"; cmd10.value = "9"; commands.push_back(cmd10);
    LRUCacheSimulator::simulate(2, commands);
}

int main() {
    cout << "===========================================" << endl;
    cout << "     ЛАБОРАТОРНАЯ РАБОТА №2" << endl;
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
