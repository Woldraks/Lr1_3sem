#pragma once
#include "../PositionEnum.h"
#include <string>
using namespace std;

struct DoubleNode {
    string key;
    DoubleNode* next;
    DoubleNode* prev;
};

struct LinkedList {
    DoubleNode* head;
    DoubleNode* tail;
};

LinkedList* CreateLinkedList();
DoubleNode* CreateDoubleNode(string key);
bool isEmpty(LinkedList* list);
void addToHead(LinkedList* list, DoubleNode* addingNode);
void addToTail(LinkedList* list, DoubleNode* addingNode);
DoubleNode* FindNodeByValue(LinkedList* list, string key);
void AddNode(Position pos, DoubleNode* addingNode, LinkedList* list, DoubleNode* node);
void RemoveNode(Position pos, LinkedList* list, DoubleNode* node);
void RemoveNodeByValue(LinkedList* list, string key);
void Print(LinkedList* list);
void PrintReversed(LinkedList* list);
void Destroy(LinkedList* list);
