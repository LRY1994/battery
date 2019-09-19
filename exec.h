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

    int time_segment;
    int layer;
    double rootT;
    int first_layer_num;


    Exec(int time_segment,double initial_T,int first_layer_num);

    vector<double> getMArray();
    double getM(int index);
    double getX(int index);
    double getY(int index);


};

#endif // EXEC_H
