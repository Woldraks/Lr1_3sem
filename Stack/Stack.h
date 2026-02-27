#pragma once
#include <string>
using namespace std;

struct StackElement {
    string key;
    StackElement* prev;
};

struct Stack {
    StackElement* last;
};

StackElement* CreateStackElement(string key);
Stack* CreateStack();
bool isEmpty(Stack* stack);
void Push(Stack* stack, StackElement* element);
string Pop(Stack* stack);
void Print(Stack* stack);
void ClearStack(Stack* stack);
