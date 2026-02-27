#pragma once
#include <string>
using namespace std;

struct Array {
    string* data;
    int capacity;
    int size;
};

Array* CreateArray(int initialCapacity);
int GetLength(Array* arr);
void ExtendArrayIfNeeded(Array* arr);
void Push(Array* arr, string key);
void Insert(Array* arr, int index, string key);
void ReplaceByIndex(Array* arr, int index, string key);
void DeleteByIndex(Array* arr, int index);
string GetByIndex(Array* arr, int index);
void Print(Array* arr);
void Destroy(Array* arr);
