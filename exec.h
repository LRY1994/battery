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

    
    double rootT;
    int first_layer_num;//第一层分裂数
    int other_layer_num;//其他层分裂数
    int layer;


    Exec(double a_rootT, int a_first_layer_num, int a_other_layer_num);

    vector<double> getMArray();
    double getM(int index);
    double getX(int index);
    double getY(int index);


};

#endif // EXEC_H
