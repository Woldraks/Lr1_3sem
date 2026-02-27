#include <iostream>
#include "Tree.h"

// Создает новый узел с заданным ключом, без потомков
BSTNode* CreateBSTNode(int key) {           
    return new BSTNode{key, nullptr, nullptr};
}

// Создает пустое BST (корень nullptr)
BST* CreateBST() {
    return new BST{nullptr};
}

// Проверяет, пустое ли дерево
bool isEmpty(BST* tree) {
    return tree->root == nullptr;
}

// Вставка узла по ключу (без повторов)
void InsertNode(BST* tree, int key) {       
    BSTNode* newNode = CreateBSTNode(key);
    
    if (isEmpty(tree)) {
        tree->root = newNode;
        return;
    }
    
    BSTNode* curr = tree->root;
    BSTNode* parent = nullptr;
    
    // Ищем позицию для вставки
    while (curr != nullptr) {
        parent = curr;
        if (key < curr->key) {      
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            cout << "Узел со значением " << key << " уже есть в дереве" << endl;
            delete newNode;
            return;
        }
    }
    
    // Вставляем новый узел в найденную позицию
    if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}

// Обход inorder (левый, корень, правый), печать ключей
void inorderPrint(BSTNode* node) {
    if (node == nullptr) return;
    inorderPrint(node->left);
    cout << node->key << " ";      
    inorderPrint(node->right);
}

// Печать дерева соответствующим обходом
void PrintInorder(BST* tree) {
    inorderPrint(tree->root);
    cout << endl;
}

// Находит узел с минимальным ключом в поддереве
BSTNode* findMinNode(BSTNode* node) {
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Поиск узла по ключу
BSTNode* FindNode(BSTNode* root, int key) {  
    BSTNode* curr = root;
    while (curr != nullptr) {
        if (key == curr->key)
            return curr;
        else if (key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nullptr;
}

// Рекурсивное удаление узла по ключу
BSTNode* findAndRemoveNode(BSTNode* root, int key) {  
    if (root == nullptr) return root;
    
    if (key < root->key) {
        root->left = findAndRemoveNode(root->left, key);
    } else if (key > root->key) {
        root->right = findAndRemoveNode(root->right, key);
    } else {
        // Случай 1: нет левого потомка
        if (root->left == nullptr) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        }
        // Случай 2: нет правого потомка
        else if (root->right == nullptr) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        // Случай 3: есть оба потомка
        BSTNode* temp = findMinNode(root->right);
        root->key = temp->key;                    
        root->right = findAndRemoveNode(root->right, temp->key);
    }
    return root;
}

// Удаление узла по значению из дерева
void RemoveNodeByValue(BST* tree, int key) {    
    tree->root = findAndRemoveNode(tree->root, key);
}

// Рекурсивная очистка дерева 
void clearBSTRec(BSTNode* node) {
    if (!node) return;
    clearBSTRec(node->left);
    clearBSTRec(node->right);
    delete node;
}

// Полное удаление дерева
void Destroy(BST* tree) {
    clearBSTRec(tree->root);
    tree->root = nullptr;
}