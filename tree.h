#ifndef TREE_H
#define TREE_H

#include "struct.h"
using namespace std;

class Tree
{
private:
    BTNode *&root;
    double rootT;// the temperature of root
    int height;//the height of the tree
    int root_current_index;//root node's corresponding current index
    double min_cost;        //result
    vector<double> min_path;//result
    

public:
    Tree(BTNode *&root,
         double initial,
         int height ,
         int root_current_index);
    void create();
    void depthFirstSearch();
    double getMinCost();
    vector<double> getPath();
};

#endif // TREE_H
