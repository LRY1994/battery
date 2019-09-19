#include <vector>
using namespace std;

#ifndef _STRUCT_H
#define _STRUCT_H

typedef struct BTNode
{
    double temperature;
    int layer;
    double all_cost;
    int SOC;
    vector<double> path;
    vector<BTNode *> children;
    BTNode(){}
} * BTree;

struct Point
{
    double x; //time
    double y; //temparature
    Point(double a,double b){ x=a;y=b;}
};

#endif // _STRUCT_H
