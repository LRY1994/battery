#ifndef TREE_H
#define TREE_H

#include "struct.h"
using namespace std;

class Tree
{
private:
    BTNode *&root;
    double rootT;// the temperature of root
    int layer;   // how many layers of the tree
    int firstLayerNum;
    double I;
public:
    Tree(BTNode *&root,
         double initial,
         int firstLayerNum,
         int layer,
         double I);
    void create();
    void depthFirstSearch();
};

#endif // TREE_H
