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

struct Current_Area{
    double current;
    int from;
    int to ;
    int dt;
    Current_Area(double a,int b,int c){ 
        current = a;
        from = b;
        to = c;
        dt = c-b + 1;
    }
};

#endif // _STRUCT_H
