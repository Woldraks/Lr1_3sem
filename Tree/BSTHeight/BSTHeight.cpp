#include "BSTHeight.h"
#include <iostream>
using namespace std;

int BSTHeightCalculator::calculateHeightRec(BSTNode* node) {
    if (node == nullptr) {
        return 0;
    }
    
    int leftHeight = calculateHeightRec(node->left);
    int rightHeight = calculateHeightRec(node->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int BSTHeightCalculator::calculateHeight(BST* tree) {
    if (tree == nullptr || tree->root == nullptr) {
        return 0;
    }
    
    return calculateHeightRec(tree->root);
}
