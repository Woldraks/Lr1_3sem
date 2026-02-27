#include "Subarrays.h"
#include "../Array.h"
#include <iostream>
#include <cmath>
using namespace std;

Array** SubarrayGenerator::generateAllSubarrays(Array* arr, int& resultCount) {
    int n = GetLength(arr);
    
    // Количество подмассивов = 2^n
    resultCount = pow(2, n);
    
    Array** result = new Array*[resultCount];
    
    // Создаем пустой подмассив (индекс 0)
    result[0] = CreateArray(8);
    
    int currentIndex = 1;
    
    // Генерируем все подмассивы
    for (int i = 0; i < n; i++) {
        int size = currentIndex;
        for (int j = 0; j < size; j++) {
            // Копируем существующий подмассив
            Array* newSubarray = CreateArray(8);
            for (int k = 0; k < GetLength(result[j]); k++) {
                Push(newSubarray, GetByIndex(result[j], k));
            }
            // Добавляем текущий элемент
            Push(newSubarray, GetByIndex(arr, i));
            result[currentIndex++] = newSubarray;
        }
    }
    
    return result;
}

void SubarrayGenerator::printAllSubarrays(Array* arr) {
    int count;
    Array** subarrays = generateAllSubarrays(arr, count);
    
    cout << "Все различные подмассивы (используя Array из 1 лабы):" << endl;
    cout << "[";
    for (int i = 0; i < count; i++) {
        cout << "{";
        for (int j = 0; j < GetLength(subarrays[i]); j++) {
            cout << GetByIndex(subarrays[i], j);
            if (j < GetLength(subarrays[i]) - 1) cout << ", ";
        }
        cout << "}";
        if (i < count - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    cout << "Всего подмассивов: " << count << endl;
    
    destroySubarrays(subarrays, count);
}

void SubarrayGenerator::destroySubarrays(Array** subarrays, int count) {
    for (int i = 0; i < count; i++) {
        Destroy(subarrays[i]);
    }
    delete[] subarrays;
}
