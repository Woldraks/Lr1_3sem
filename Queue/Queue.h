#pragma once
#include <string>
using namespace std;

struct QueueElement {
    string key;
    QueueElement* next;
};

struct Queue {
    QueueElement* first;
    QueueElement* last;
};

QueueElement* CreateQueueElement(string key);
Queue* CreateQueue();
bool isEmpty(Queue* queue);
void Push(Queue* queue, QueueElement* element);
string Pop(Queue* queue);
void Print(Queue* queue);
void ClearQueue(Queue* queue);
