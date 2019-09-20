#ifndef CURRENT_H
#define CURRENT_H

#include "struct.h"
#include <cstring>
#include <vector>
using namespace  std;

class Current
{
public:  
   //read data 
    vector<Point> readData(const char* fileName,int from ,int to);

    //process data
    vector<Current_Area>  processData(vector<Point>data);


};

#endif // CURRENT_H
