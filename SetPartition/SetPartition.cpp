#include "SetPartition.h"
#include "../Array/Array.h"
#include "../HashSet/HashSet.h"
#include <iostream>
#include <string>
using namespace std;

bool allUsed(bool* used, int count) {
    for (int i = 0; i < count; i++) {
        if (!used[i]) return false;
    }
    return true;
}


static bool canPartition(int* elements, int count, int targetSum,
                        bool* used, int* current, int& currentSize,
                        Array** result, int& resultIndex, 
                        int startIndex, int currentSum) {
    
    // 1: Нашли текущее подмножество
    if (currentSum == targetSum) {
        // Сохраняем текущее подмножество
        Array* subset = CreateArray(currentSize);
        for (int i = 0; i < currentSize; i++) {
            Push(subset, to_string(current[i]));
        }
        result[resultIndex++] = subset;
       
        if (allUsed(used, count)) {
            return true;
        }
        
        //  Ищем следующий неиспользованный элемент для нового подмножества
        for (int i = 0; i < count; i++) {
            if (!used[i]) {
                
                int newSize = 0;
                return canPartition(elements, count, targetSum, used,
                                   current, newSize,  
                                   result, resultIndex, i, 0);
            }
        }
        return true;
    }
    
    //Пробуем добавить элементы
    for (int i = startIndex; i < count; i++) {
        if (!used[i] && currentSum + elements[i] <= targetSum) {
            // Пробуем взять этот элемент
            used[i] = true;
            current[currentSize] = elements[i];
            
            //увеличиваем размер перед вызовом
            currentSize++;
            
            if (canPartition(elements, count, targetSum, used,
                            current, currentSize,  
                            result, resultIndex, i + 1,
                            currentSum + elements[i])) {
                return true;
            }
            
            // Не получилось - возвращаемся 
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

// ============================================================================
// ОСНОВНАЯ ФУНКЦИЯ РАЗБИЕНИЯ
// ============================================================================
PartitionResult SetPartitioner::partitionIntoEqualSums(Array* set, int targetSum) {
    PartitionResult result;
    result.possible = false;
    result.targetSum = targetSum;
    result.subsets = nullptr;
    result.subsetCount = 0;
    
    int n = GetLength(set);
    if (n == 0) return result;
    
    // Преобразуем Array в массив чисел
    int* elements = new int[n];
    for (int i = 0; i < n; i++) {
        elements[i] = stoi(GetByIndex(set, i));
    }
    
    // ортируем по убыванию (для оптимизации)
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
    
    // Проверяем, делится ли общая сумма на targetSum
    if (totalSum % targetSum != 0) {
        cout << totalSum << " не делится на " << targetSum << endl;
        delete[] elements;
        return result;
    }
    
    int numSubsets = totalSum / targetSum;  // Сколько должно быть подмножеств
    cout << "  Должно получиться подмножеств: " << numSubsets << endl;
    
    // Создаем вспомогательные массивы
    bool* used = new bool[n];
    for (int i = 0; i < n; i++) used[i] = false;
    
    int* current = new int[n];
    int currentSize = 0;
    
    // Массив для результатов
    result.subsets = new Array*[numSubsets];
    result.subsetCount = 0;
    
    // 🟢 Шаг 6: Запускаем поиск (начинаем с индекса 0 и суммы 0)
    cout << "  Начинаем поиск..." << endl;
    
    if (canPartition(elements, n, targetSum, used, current, currentSize,
                     result.subsets, result.subsetCount, 0, 0)) {
        result.possible = true;
        cout << "  Поиск успешен!" << endl;
    } else {
        cout << "   Поиск не удался" << endl;
        // Если не получилось - удаляем созданные подмножества
        for (int i = 0; i < result.subsetCount; i++) {
            Destroy(result.subsets[i]);
        }
        delete[] result.subsets;
        result.subsets = nullptr;
        result.subsetCount = 0;
    }
    
    // Очищаем временные массивы
    delete[] elements;
    delete[] used;
    delete[] current;
    
    return result;
}

// ============================================================================
// ФУНКЦИЯ ПЕЧАТИ РЕЗУЛЬТАТА
// ============================================================================
void SetPartitioner::printPartition(const PartitionResult& result) {
    if (!result.possible) {
        cout << "Невозможно разбить множество на подмножества с суммой " 
             << result.targetSum << endl;
        return;
    }
    
    cout << "Разбиение на " << result.subsetCount 
         << " подмножеств с суммой " << result.targetSum << ":" << endl;
    
    for (int i = 0; i < result.subsetCount; i++) {
        cout << "   Подмножество " << i + 1 << ": {";
        for (int j = 0; j < GetLength(result.subsets[i]); j++) {
            cout << GetByIndex(result.subsets[i], j);
            if (j < GetLength(result.subsets[i]) - 1) cout << ", ";
        }
        cout << "}" << endl;
    }
}

// ============================================================================
// ФУНКЦИЯ ОЧИСТКИ ПАМЯТИ
// ============================================================================
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

// ============================================================================
// ПОИСК ДВУХ МНОЖЕСТВ С МАКСИМАЛЬНЫМ ПЕРЕСЕЧЕНИЕМ
// ============================================================================
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
        
        cout << "   Множество " << bestI + 1 << ": {";
        for (int i = 0; i < GetLength(sets[bestI]); i++) {
            cout << GetByIndex(sets[bestI], i);
            if (i < GetLength(sets[bestI]) - 1) cout << ", ";
        }
        cout << "}" << endl;
        
        cout << "   Множество " << bestJ + 1 << ": {";
        for (int i = 0; i < GetLength(sets[bestJ]); i++) {
            cout << GetByIndex(sets[bestJ], i);
            if (i < GetLength(sets[bestJ]) - 1) cout << ", ";
        }
        cout << "}" << endl;
        
        cout << "   Количество общих элементов: " << maxIntersection << endl;
    }
}