#include "SetPartition.h"
#include "../Array/Array.h"
#include "../HashSet/HashSet.h"
#include <iostream>
#include <string>
using namespace std;

// Рекурсивная функция для разбиения
static bool canPartition(int* elements, int count, int targetSum,
                        bool* used, int* current, int& currentSize,
                        Array** result, int& resultIndex, int currentSum) {
    if (currentSum == targetSum) {
        // Создаем подмассив из текущей комбинации
        Array* subset = CreateArray(currentSize);
        for (int i = 0; i < currentSize; i++) {
            Push(subset, to_string(current[i]));
        }
        result[resultIndex++] = subset;
        
        currentSize = 0;
        return true;
    }
    
    for (int i = 0; i < count; i++) {
        if (!used[i] && currentSum + elements[i] <= targetSum) {
            used[i] = true;
            current[currentSize++] = elements[i];
            
            if (canPartition(elements, count, targetSum, used, current, currentSize,
                            result, resultIndex, currentSum + elements[i])) {
                return true;
            }
            
            currentSize--;
            used[i] = false;
            
            // Пропускаем дубликаты
            while (i + 1 < count && elements[i] == elements[i + 1]) {
                i++;
            }
        }
    }
    return false;
}

// Основная функция разбиения
PartitionResult SetPartitioner::partitionIntoEqualSums(Array* set, int targetSum) {
    PartitionResult result;
    result.possible = false;
    result.targetSum = targetSum;
    result.subsets = nullptr;
    result.subsetCount = 0;
    
    int n = GetLength(set);
    if (n == 0) return result;
    
    // Преобразуем Array в обычный массив int для удобства
    int* elements = new int[n];
    for (int i = 0; i < n; i++) {
        elements[i] = stoi(GetByIndex(set, i));
    }
    
    // Сортировка пузырьком по убыванию
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (elements[j] < elements[j + 1]) {
                int temp = elements[j];
                elements[j] = elements[j + 1];
                elements[j + 1] = temp;
            }
        }
    }
    
    // Вычисляем общую сумму
    int totalSum = 0;
    for (int i = 0; i < n; i++) {
        totalSum += elements[i];
    }
    
    if (totalSum % targetSum != 0) {
        delete[] elements;
        return result;
    }
    
    int numSubsets = totalSum / targetSum;
    bool* used = new bool[n];
    for (int i = 0; i < n; i++) used[i] = false;
    
    int* current = new int[n];
    int currentSize = 0;
    
    result.subsets = new Array*[numSubsets];
    result.subsetCount = 0;
    
    if (canPartition(elements, n, targetSum, used, current, currentSize,
                     result.subsets, result.subsetCount, 0)) {
        result.possible = true;
    }
    
    delete[] elements;
    delete[] used;
    delete[] current;
    
    return result;
}

// Поиск двух множеств с максимальным пересечением
void SetPartitioner::findMaxIntersection(Array** sets, int setCount) {
    if (setCount < 2) {
        cout << "Нужно как минимум 2 множества для сравнения" << endl;
        return;
    }
    
    int maxIntersection = -1;
    int bestI = -1, bestJ = -1;
    
    // Сравниваем все пары множеств
    for (int i = 0; i < setCount; i++) {
        for (int j = i + 1; j < setCount; j++) {
            // Находим пересечение
            int intersection = 0;
            
            // Для каждого элемента из первого множества проверяем, есть ли он во втором
            for (int k = 0; k < GetLength(sets[i]); k++) {
                string elem = GetByIndex(sets[i], k);
                
                // Проверяем наличие элемента во втором множестве
                for (int l = 0; l < GetLength(sets[j]); l++) {
                    if (GetByIndex(sets[j], l) == elem) {
                        intersection++;
                        break;
                    }
                }
            }
            
            if (intersection > maxIntersection) {
                maxIntersection = intersection;
                bestI = i;
                bestJ = j;
            }
        }
    }
    
    if (maxIntersection >= 0) {
        cout << "Множества с максимальным пересечением:" << endl;
        cout << "Множество " << bestI + 1 << ": {";
        for (int i = 0; i < GetLength(sets[bestI]); i++) {
            cout << GetByIndex(sets[bestI], i);
            if (i < GetLength(sets[bestI]) - 1) cout << ", ";
        }
        cout << "}" << endl;
        
        cout << "Множество " << bestJ + 1 << ": {";
        for (int i = 0; i < GetLength(sets[bestJ]); i++) {
            cout << GetByIndex(sets[bestJ], i);
            if (i < GetLength(sets[bestJ]) - 1) cout << ", ";
        }
        cout << "}" << endl;
        
        cout << "Количество общих элементов: " << maxIntersection << endl;
    }
}

void SetPartitioner::printPartition(const PartitionResult& result) {
    if (!result.possible) {
        cout << "Невозможно разбить множество на подмножества с суммой " 
             << result.targetSum << endl;
        return;
    }
    
    cout << "Разбиение на подмножества с суммой " << result.targetSum 
         << " (используя Array из 1 лабы):" << endl;
    for (int i = 0; i < result.subsetCount; i++) {
        cout << "Подмножество " << i + 1 << ": {";
        for (int j = 0; j < GetLength(result.subsets[i]); j++) {
            cout << GetByIndex(result.subsets[i], j);
            if (j < GetLength(result.subsets[i]) - 1) cout << ", ";
        }
        cout << "}" << endl;
    }
}

void SetPartitioner::destroyPartition(PartitionResult& result) {
    if (result.subsets) {
        for (int i = 0; i < result.subsetCount; i++) {
            Destroy(result.subsets[i]);
        }
        delete[] result.subsets;
        result.subsets = nullptr;
        result.subsetCount = 0;
    }
}
