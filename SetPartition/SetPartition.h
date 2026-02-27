#pragma once
#include "../Array/Array.h"
#include "../HashSet/HashSet.h"

struct PartitionResult {
    Array** subsets;        // Массив массивов (подмножества)
    int subsetCount;        // Количество подмножеств
    bool possible;          // Возможно ли разбиение
    int targetSum;          // Целевая сумма
};

class SetPartitioner {
public:
    // Разбиение множества на подмножества с равной суммой
    static PartitionResult partitionIntoEqualSums(Array* set, int targetSum);
    
    // Поиск двух множеств с максимальным количеством общих элементов
    static void findMaxIntersection(Array** sets, int setCount);
    
    // Вспомогательные функции
    static void printPartition(const PartitionResult& result);
    static void destroyPartition(PartitionResult& result);
};
