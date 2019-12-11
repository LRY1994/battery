#ifndef CURRENT_H
#define CURRENT_H

#include "struct.h"
#include <cstring>
#include <vector>
using namespace  std;

class Current
{
    private:
        vector<Current_Area> data;
        vector<int> depth;
        vector<Current_Area> Raw;
        //read data
        vector<Point> readData(const char *fileName, int from, int to);
        //process data
        void processData(vector<Point> data);
        //degree of each area
        void setDegree();
        //tree height
        void setDepth();

    public:
        Current(const char *fileName, int from, int to);

        vector<int> getDepth();
        vector<Current_Area> getData();
        vector<Current_Area> getpoint();
};

#endif // CURRENT_H
