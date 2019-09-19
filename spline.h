#ifndef SPLINE_H
#define SPLINE_H

#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include "struct.h"
using namespace std;

class Spline
{
private:
    double x[1000000];
    double y[1000000];
    double afa[1000000];
    double bta[1000000];
    double  h[1000000];
    double a[1000000];
    double b[1000000];
    double m[1000000];
    int n;
public:
    vector<double> interpolation(vector<Point> pointList,int option);
    double getValue(double xx);
};

#endif // SPLINE_H
