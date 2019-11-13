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
    int height;//树高度
    int root_current_index;
    double min_cost;        //result
    vector<double> min_path;//result
    

public:
    Tree(BTNode *&root,
         double initial,
         int firstLayerNum,
         int other_layer_num,
         int height ,
         int root_current_index);
    void create();
    void depthFirstSearch();
    double getMinCost();
    vector<double> getPath();
};

#endif // TREE_H
