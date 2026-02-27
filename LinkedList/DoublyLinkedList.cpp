#include <iostream>
#include "DoublyLinkedList.h"

// Создание пустого двусвязного списка
LinkedList* CreateLinkedList() {
    return new LinkedList{nullptr, nullptr};
}

// Создание нового узла с заданным значением ключа
DoubleNode* CreateDoubleNode(string key) {
    return new DoubleNode{key, nullptr, nullptr};
}

// Проверка, пустой ли список
bool isEmpty(LinkedList* list) {
    return list->head == nullptr;
}

// Добавление узла в начало списка
void addToHead(LinkedList* list, DoubleNode* addingNode) {
    addingNode->next = list->head;
    addingNode->prev = nullptr;
    if (list->head != nullptr)
        list->head->prev = addingNode;
    list->head = addingNode;

    // Если список был пуст, обновляем хвост
    if (list->tail == nullptr)
        list->tail = addingNode;
}

// Добавление узла в конец списка
void addToTail(LinkedList* list, DoubleNode* addingNode) {
    addingNode->prev = list->tail;
    addingNode->next = nullptr;
    if (list->tail != nullptr)
        list->tail->next = addingNode;
    list->tail = addingNode;

    // Если список был пуст, обновляем голову
    if (list->head == nullptr)
        list->head = addingNode;
}

// Поиск узла по значению ключа, возвращает nullptr, если не найден
DoubleNode* FindNodeByValue(LinkedList* list, string key) {
    if (isEmpty(list)) {
        cout << "Список пуст!" << endl;
        return nullptr;
    }

    DoubleNode* curr = list->head;
    while (curr != nullptr && curr->key != key) {
        curr = curr->next;
    }
    if (curr != nullptr && curr->key == key)
        return curr;

    cout << "Не удалось найти узел со значением " << key << endl;
    return nullptr;
}

// Добавление узла в список в указанной позиции относительно node 
void AddNode(Position pos, DoubleNode* addingNode, LinkedList* list, DoubleNode* node) {
    if (isEmpty(list)) {
        // Если список пустой - добавляем первый узел и обновляем голову и хвост
        list->head = list->tail = addingNode;
        return;
    }

    switch (pos) {
        case Position::Before: {
            if (node == nullptr) {
                cout << "При добавлении узла ДО нужно указать узел, перед которым нужно добавить узел." << endl;
                return;
            }
            if (node == list->head) {
                addToHead(list, addingNode);
                break;
            }

            // Связываем добавляемый узел с соседями
            addingNode->next = node;
            addingNode->prev = node->prev;
            node->prev->next = addingNode;
            node->prev = addingNode;
            break;
        }
        case Position::After: {
            if (node == nullptr) {
                cout << "При добавлении узла ПОСЛЕ нужно указать узел, после которого нужно добавить узел." << endl;
                return;
            }
            if (node == list->tail) {
                addToTail(list, addingNode);
                break;
            }

            // Связываем добавляемый узел с соседями
            addingNode->prev = node;
            addingNode->next = node->next;
            node->next->prev = addingNode;
            node->next = addingNode;
            break;
        }
        case Position::Head: {
            addToHead(list, addingNode);
            break;
        }
        case Position::Tail: {
            addToTail(list, addingNode);
            break;
        }
        default:
            cout << "Неверная позиция для добавления." << endl;
            return;
    }

    
}

// Удаление узла из списка в позициях относительно node 
void RemoveNode(Position pos, LinkedList* list, DoubleNode* node) {
    if (isEmpty(list)) {
        cout << "Список пуст!" << endl;
        return;
    }

    switch (pos) {
        case Position::Before: {
            if (node == nullptr) {
                cout << "При удалении узла ДО нужно указать узел, перед которым нужно удалить узел." << endl;
                return;
            }
            if (node == list->head) {
                cout << "Невозможно удалить узел ДО головы списка." << endl;
                return;
            }

            DoubleNode* prevNode = node->prev;
            if (prevNode->prev == nullptr) { // prevNode - голова
                list->head = node;
                node->prev = nullptr;
            } else {
                prevNode->prev->next = node;
                node->prev = prevNode->prev;
            }
            delete prevNode;
            break;
        }
        case Position::After: {
            if (node == nullptr) {
                cout << "При удалении узла ПОСЛЕ нужно указать узел, после которого нужно удалить узел." << endl;
                return;
            }
            if (node == list->tail) {
                cout << "Нечего удалять, мы в хвосте." << endl;
                return;
            }

            DoubleNode* nextPtr = node->next;
            if (nextPtr == list->tail) {
                list->tail = node;
                node->next = nullptr;
                delete nextPtr;
            } else {
                node->next = nextPtr->next;
                nextPtr->next->prev = node;
                delete nextPtr;
            }
            break;
        }
        case Position::Head: {
            DoubleNode* headPtr = list->head;
            if (list->head == list->tail) { // Одни элемент в списке
                list->head = list->tail = nullptr;
            } else {
                list->head = headPtr->next;
                list->head->prev = nullptr;
            }
            delete headPtr;
            break;
        }
        case Position::Tail: {
            DoubleNode* tailPtr = list->tail;
            if (list->head == tailPtr) { // Одни элемент в списке
                list->head = list->tail = nullptr;
            } else {
                list->tail = tailPtr->prev;
                list->tail->next = nullptr;
            }
            delete tailPtr;
            break;
        }
        default:
            cout << "Неверная позиция для удаления." << endl;
            return;
    }

   
}

// Удаление узла по значению ключа (удаляется первый найденный узел)
void RemoveNodeByValue(LinkedList* list, string key) {
    DoubleNode* foundNode = FindNodeByValue(list, key);
    if (!foundNode) return;

    if (foundNode == list->head) {
        RemoveNode(Position::Head, list, foundNode);
        return;
    }
    if (foundNode == list->tail) {
        RemoveNode(Position::Tail, list, foundNode);
        return;
    }

    // Удаляем узел из середины списка
    foundNode->prev->next = foundNode->next;
    foundNode->next->prev = foundNode->prev;
    delete foundNode;

    cout << "Узел со значением " << key << " успешно удалён." << endl;
}

// Вывод элементов списка в прямом порядке
void Print(LinkedList* list) {
    cout << "Список: [ ";
    DoubleNode* curr = list->head;
    while (curr != nullptr) {
        cout << curr->key << " ";
        curr = curr->next;
    }
    cout << "]" << endl;
}

// Вывод элементов списка в обратном порядке
void PrintReversed(LinkedList* list) {
    cout << "Список в обратном порядке: [ ";
    DoubleNode* curr = list->tail;
    while (curr != nullptr) {
        cout << curr->key << " ";
        curr = curr->prev;
    }
    cout << "]" << endl;
}

// Полное удаление всех узлов и очистка списка
void Destroy(LinkedList* list) {
    DoubleNode* curr = list->head;
    while(curr != nullptr) {
        DoubleNode* tmpNextPtr = curr->next;
        delete curr;
        curr = tmpNextPtr;
    }
    list->head = nullptr;
    list->tail = nullptr;
}