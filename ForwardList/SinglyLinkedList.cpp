#include <iostream>
#include "SinglyLinkedList.h"

// Создание пустого списка
ForwardList* CreateForwardList() {
    return new ForwardList{nullptr};
}

// Создание узла с заданным значением
SingleNode* CreateSingleNode(string key) {
    return new SingleNode{key, nullptr};
}

// Проверка пустоты списка
bool isEmpty(ForwardList* list) {
    return list->head == nullptr;
}

// Добавить узел в голову списка
void addToHead(ForwardList* list, SingleNode* addingNode) {
    addingNode->next = list->head;
    list->head = addingNode;
}

// Добавить узел в хвост списка
void addToTail(ForwardList* list, SingleNode* addingNode) {
    addingNode->next = nullptr;
    if (list->head == nullptr) {
        list->head = addingNode;
        return;
    }
    SingleNode* curr = list->head;
    while (curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = addingNode;
}

// Найти узел, предшествующий заданному
SingleNode* findPrevNode(ForwardList* list, SingleNode* node) {
    if (list->head == nullptr || node == list->head) return nullptr;

    SingleNode* curr = list->head;
    while (curr != nullptr && curr->next != node) {
        curr = curr->next;
    }
    return curr; // Вернёт nullptr если не найден
}

// Поиск узла по значению
SingleNode* FindNodeByValue(ForwardList* list, string key) {
    SingleNode* curr = list->head;
    while (curr != nullptr) {
        if (curr->key == key)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

// Добавление узла по позиции
void AddNode(Position pos, SingleNode* addingNode, ForwardList* list, SingleNode* node) {
    if (isEmpty(list)) {
        list->head = addingNode;
        return;
    }

    switch (pos) {
        case Position::Before: {
            if (node == nullptr) {
                cout << "При добавлении до нужно указать целевой узел." << endl;
                return;
            }
            if (node == list->head) {
                addToHead(list, addingNode);
            } else {
                SingleNode* prev = findPrevNode(list, node);
                if (!prev) {
                    cout << "Целевой узел не найден в списке." << endl;
                    return;
                }
                addingNode->next = node;
                prev->next = addingNode;
            }
            break;
        }

        case Position::After: {
            if (node == nullptr) {
                cout << "При добавлении после нужно указать целевой узел." << endl;
                return;
            }
            addingNode->next = node->next;
            node->next = addingNode;
            break;
        }

        case Position::Head:
            addToHead(list, addingNode);
            break;

        case Position::Tail:
            addToTail(list, addingNode);
            break;
    }
    
}

// Удаление узла из списка по позиции
void RemoveNode(Position pos, ForwardList* list, SingleNode* node) {
    if (isEmpty(list)) {
        cout << "Список пуст!" << endl;
        return;
    }

    switch (pos) {
        case Position::Before: {
            if (node == nullptr) {
                cout << "При удалении до нужно указать целевой узел." << endl;
                return;
            }
            if (node == list->head) {
                cout << "Не существует узла перед головой." << endl;
                return;
            }
            SingleNode* prev = findPrevNode(list, node);
            if (!prev) {
                cout << "Целевой узел не найден." << endl;
                return;
            }
            SingleNode* beforePrev = findPrevNode(list, prev);
            if (beforePrev == nullptr) {
                list->head = node;
            } else {
                beforePrev->next = node;
            }
            delete prev;
            break;
        }

        case Position::After: {
            if (node == nullptr) {
                cout << "При удалении после нужно указать целевой узел." << endl;
                return;
            }
            if (node->next == nullptr) {
                cout << "Нет узла после хвоста." << endl;
                return;
            }
            SingleNode* toDelete = node->next;
            node->next = toDelete->next;
            delete toDelete;
            break;
        }

        case Position::Head: {
            SingleNode* toDelete = list->head;
            list->head = toDelete->next;
            delete toDelete;
            break;
        }

        case Position::Tail: {
            if (node == list->head && list->head->next == nullptr) {
                // Один элемент
                delete list->head;
                list->head = nullptr;
                break;
            }
            
            SingleNode* curr = list->head;
            while (curr->next != nullptr) {
                curr = curr->next;
            }
            SingleNode* tail = curr;
            SingleNode* prev = findPrevNode(list, tail);
            if (prev) {
                delete tail;
                tail = prev;
                prev->next = nullptr;
            }
            break;
        }
    }
    
}

// Удаление узла по значению
void RemoveNodeByValue(ForwardList* list, string key) {
    SingleNode* foundNode = FindNodeByValue(list, key);
    if (!foundNode) {
        cout << "Узел со значением " << key << " не найден." << endl;
        return;
    }
    if (foundNode == list->head) {
        RemoveNode(Position::Head, list, foundNode);
    } else {
        SingleNode* prev = findPrevNode(list, foundNode);
        if (!prev) {
            cout << "Ошибка: не найден предыдущий узел." << endl;
            return;
        }
        prev->next = foundNode->next;
        delete foundNode;
        
    }
}

// Вывод списка в прямом порядке
void Print(ForwardList* list) {
    cout << "Список: [ ";
    SingleNode* curr = list->head;
    while (curr != nullptr) {
        cout << curr->key << " ";
        curr = curr->next;
    }
    cout << "]" << endl;
}

// Вспомогательная рекурсивная функция для вывода в обратном порядке
void PrintReversedRec(SingleNode* node) {
    if (node == nullptr) return;
    PrintReversedRec(node->next);
    cout << node->key << " ";
}

// Вывод списка в обратном порядке (рекурсивно)
void PrintReversed(ForwardList* list) {
    cout << "Список в обратном порядке: [ ";
    PrintReversedRec(list->head);
    cout << "]" << endl;
}

void Destroy(ForwardList* list) {
    if (!list) return;
    SingleNode* current = list->head;
    while (current) {
        SingleNode* next = current->next;
        delete current;
        current = next;
    }
    delete list;
}