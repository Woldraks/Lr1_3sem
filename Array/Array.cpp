#include <iostream>
#include "Array.h"

// Создает новый массив с начальной емкостью
Array* CreateArray(int initialCapacity) {
    Array* arr = new Array;
    arr->capacity = initialCapacity;
    arr->size = 0;
    arr->data = new string[arr->capacity];
    return arr;
}

// Возвращает текущую длину массива (количество элементов)
int GetLength(Array* arr) {
    return arr->size;
}

// Увеличивает ёмкость массива вдвое, если текущий размер достиг ёмкости
void ExtendArrayIfNeeded(Array* arr) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;

        string* newData = new string[arr->capacity];
        for (int i = 0; i < arr->size; i++) {
            newData[i] = arr->data[i];
        }

        delete [] arr->data;
        arr->data = newData;
    }
}

// Добавляет элемент в конец массива, расширяя его при необходимости
void Push(Array* arr, string key) {
    ExtendArrayIfNeeded(arr);
    arr->data[arr->size] = key;
    arr->size++;
}

// Вставляет элемент по указанному индексу
void Insert(Array* arr, int index, string key) {
    if (index < 0) {
        cout << "Индекс не может быть отрицательным." << endl;
        return;
    }

    ExtendArrayIfNeeded(arr);

    // если индекс указывает на конец массива или больше - добавить в конец
    if (arr->size <= index) {
        Push(arr, key);
        return;
    }

    // сдвиги элементов справа, чтобы освободить место для вставки
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i-1];
    }
    arr->data[index] = key;

    arr->size++;
}

// Заменяет элемент по индексу, если индекс в пределах массива
void ReplaceByIndex(Array* arr, int index, string key) {
    if (index < 0 || index >= arr->size) {
        cerr << "Выход за пределы массива!" << endl;
        return;
    }
    arr->data[index] = key;
}

// Удаляет элемент по индексу с сдвигом остальных элементов влево
void DeleteByIndex(Array* arr, int index) {
    if (index < 0 || index >= arr->size) {
        cerr << "Выход за пределы массива!" << endl;
        return;
    }

    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
}

// Получает элемент по индексу, возвращает -1 и выводит ошибку, если индекс некорректен
string GetByIndex(Array* arr, int index) {
    if (0 <= index && index < arr->size) return arr->data[index];
    cerr << "Выход за пределы массива!" << endl;
    return "None";
}

// Выводит содержимое массива
void Print(Array* arr) {
    if (arr->size == 0) {
        cout << "Массив пуст!" << endl;
        return;
    }

    cout << "Массив: [";
    for (int i = 0; i < arr->size; i++) {
        cout << " " << GetByIndex(arr, i); 
    }
    cout << " ]" << endl;
}

// Освобождает память, выделенную под массив
void Destroy(Array* arr) {
    delete [] arr->data;
    delete arr;
}