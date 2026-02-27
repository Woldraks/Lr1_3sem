#include <iostream>
#include "Stack.h"

// Создает новый элемент стека
StackElement* CreateStackElement(string key) {
    return new StackElement{key, nullptr};
}

// Создает пустой стек
Stack* CreateStack() {
    return new Stack{nullptr};
}

// Проверяет, пуст ли стек
bool isEmpty(Stack* stack) {
    return stack->last == nullptr;
}

// Добавляет элемент в вершину стека
void Push(Stack* stack, StackElement* element) {
    element->prev = stack->last;
    stack->last = element;
}

// Удаляет элемент с вершины стека
string Pop(Stack* stack) {
    if (isEmpty(stack)) {
        cout << "Стек пуст!" << endl;
        return "None";
    }

    StackElement* lastPtr = stack->last;
    stack->last = lastPtr->prev;
    string popedValue = lastPtr->key;
    delete lastPtr;
    return popedValue;
}

// Выводит содержимое стека от вершины к основанию
void Print(Stack* stack) {
    cout << "Стек (сверху вниз): [ ";
    for (StackElement* curr = stack->last; curr != nullptr; curr = curr->prev) {
        cout << curr->key << " ";
    }
    cout << "]" << endl;
}

// Очистка стека с освобождением памяти
void ClearStack(Stack* stack) {
    while (!isEmpty(stack)) {
        Pop(stack);
    }
}