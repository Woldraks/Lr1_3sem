#pragma once
#include "../Tree.h"

class BSTHeightCalculator {
public:
    static int calculateHeight(BST* tree);
    static int calculateHeightRec(BSTNode* node);
};
