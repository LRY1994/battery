#ifndef EXEC_H
#define EXEC_H

#include <vector>
#include "struct.h"
using namespace std;
class Exec
{

public: 
    vector<double> mArray;
    vector<Point> point;
    vector<double> pointX;
    vector<double> pointY;
    double all_min_cost ;
    vector<double> all_min_path;

    double rootT;
    int first_layer_num;//第一层分裂数
    int other_layer_num;//其他层分裂数
    


    Exec(char *file, int fromPos, int toPos,double a_rootT, int a_first_layer_num, int a_other_layer_num);
    void buildOneTree(double T, int height, int current_index);
    void buildMultiTree();
    void makePoints();
    vector<int> getDepth();//根据电流划分高度
    double getAllCost();
    // double getM(int index);
    double getX(int index);
    double getY(int index);

};

#endif // EXEC_H
