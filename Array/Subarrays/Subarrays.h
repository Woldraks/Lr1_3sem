#pragma once
#include "../Array.h"

class SubarrayGenerator {
public:
    // Генерирует все подмассивы, возвращает массив массивов
    static Array** generateAllSubarrays(Array* arr, int& resultCount);
    
    // Печатает все подмассивы
    static void printAllSubarrays(Array* arr);
    
    // Освобождает память
    static void destroySubarrays(Array** subarrays, int count);
};
