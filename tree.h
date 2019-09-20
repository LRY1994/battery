#ifndef TREE_H
#define TREE_H

#include "struct.h"
using namespace std;

class Tree
{
private:
    BTNode *&root;
    double rootT;// the temperature of root
    int first_layer_num;  
    int other_layer_num;
    int layer;
public:
    Tree(BTNode *&root,
         double initial,
         int firstLayerNum,
         int other_layer_num,
         int layer);
    void create();
    void depthFirstSearch();
};

#endif // TREE_H
