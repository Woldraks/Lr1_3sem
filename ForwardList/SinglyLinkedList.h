#pragma once
#include "../PositionEnum.h"
#include <string>
using namespace std;

struct SingleNode {
    string key;
    SingleNode* next;
};

struct ForwardList {
    SingleNode* head;
};

ForwardList* CreateForwardList();
SingleNode* CreateSingleNode(string key);
bool isEmpty(ForwardList* list);
void addToHead(ForwardList* list, SingleNode* addingNode);
void addToTail(ForwardList* list, SingleNode* addingNode);
SingleNode* findPrevNode(ForwardList* list, SingleNode* node);
SingleNode* FindNodeByValue(ForwardList* list, string key);
void AddNode(Position pos, SingleNode* addingNode, ForwardList* list, SingleNode* node);
void RemoveNodeByValue(ForwardList* list, string key);
void RemoveNode(Position pos, ForwardList* list, SingleNode* node);
void Print(ForwardList* list);
void PrintReversedRec(SingleNode* node);
void PrintReversed(ForwardList* list);
void Destroy(ForwardList* list);
