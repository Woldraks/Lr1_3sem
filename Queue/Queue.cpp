#include <iostream>
#include "Queue.h"

// Создать новый элемент очереди
QueueElement* CreateQueueElement(string key) {
    return new QueueElement{key, nullptr};
}

// Создать пустую очередь
Queue* CreateQueue() {
    return new Queue{nullptr, nullptr};
}

// Проверка, пуста ли очередь
bool isEmpty(Queue* queue) {
    return queue->first == nullptr;
}

// Добавить элемент в конец очереди
void Push(Queue* queue, QueueElement* element) {
    element->next = nullptr; // новая последняя ссылка всегда nullptr

    if (isEmpty(queue)) {
        queue->first = queue->last = element;
        return;
    }

    queue->last->next = element;
    queue->last = element;
}

// Извлечь (удалить) элемент из начала очереди
string Pop(Queue* queue) {
    if (isEmpty(queue)) {
        cout << "Очередь пуста!" << endl;
        return "None";
    }

    QueueElement* firstPtr = queue->first;
    queue->first = firstPtr->next;
    if (queue->first == nullptr) {
        // Если очередь стала пустой, обновляем указатель last
        queue->last = nullptr;
    }
    string popedValue = firstPtr->key;
    delete firstPtr;
    return popedValue;
}

// Вывести содержимое очереди
void Print(Queue* queue) {
    cout << "Очередь: [ ";
    for (QueueElement* curr = queue->first; curr != nullptr; curr = curr->next) {
        cout << curr->key << " ";
    }
    cout << "]" << endl;
}

// Очистить очередь и освободить память
void ClearQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        Pop(queue);
    }
}