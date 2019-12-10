#include <vector>
using namespace std;

#ifndef _STRUCT_H
#define _STRUCT_H

typedef struct BTNode
{
    double temperature;
    int current_index; //电流区间，根节点到其子节点为第0个电流区间，current_index为0
    double all_cost;
    double SOC;
    vector<double> path;
    vector<BTNode *> children;
    BTNode(){};
    BTNode(double t, int ci,  double soc)
    {
        temperature = t;
        current_index = ci;
        all_cost = 0;
        SOC = soc;
    }
} * BTree;

struct Point
{
    double x; //time
    double y; //temparature
    Point(double a,double b){ x=a;y=b;}
};

struct Current_Area{
    double current;//电流
    int from;//时刻
    int to ;//时刻
    int dt;
    int degree;
    Current_Area(double a,int b,int c){ 
        current = a;
        from = b;
        to = c;
        dt = c-b;
        degree = 0;
    }
};

#endif // _STRUCT_H
