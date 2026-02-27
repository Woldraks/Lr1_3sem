#pragma once
#include <string>
using namespace std;

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
};

struct BST {
    BSTNode* root;
};

BSTNode* CreateBSTNode(int key);
BST* CreateBST();
bool isEmpty(BST* tree);
void InsertNode(BST* tree, int key);
void inorderPrint(BSTNode* node);
void PrintInorder(BST* tree);
BSTNode* findMinNode(BSTNode* node);
BSTNode* FindNode(BSTNode* root, int key);
BSTNode* findAndRemoveNode(BSTNode* root, int key);
void RemoveNodeByValue(BST* tree, int key);
void clearBSTRec(BSTNode* node);
void Destroy(BST* tree);
