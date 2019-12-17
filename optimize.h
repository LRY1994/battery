#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "struct.h"
#include <vector>
using namespace std;
struct Line
{
    double a;
    double b;
    double c;
    vector<double> y;
    double last_soc;
    double cost;

    Line(double aa, double bb, double cc, double ccost, double last_socc, vector<double> yy)
    {
        a = aa;
        b = bb;
        c = cc;
        y = yy;
  
        cost = ccost;
        last_soc = last_socc;
    }
    Line(){}
};
class Optimize
{

  private:
    vector<double> pointX;
    vector<double> pointY;
    vector<Line> opt_lines;
    double opt_min_cost;

    void three(double i, double j, double k);
    vector<Line> spline_three(vector<Point> pointList, int first_current_index, double last_soc);

    vector<Line> fitting_three(double x[], double y[], int first_current_index, double last_soc);
    double cal_line_cost(
        double a, double b, double c,
        double from_t,
        double to_t,
        int current_index,
        double &last_soc,
        vector<double> &yy);

public:
    Optimize(vector<double> pointX, vector<double> pointY);
    void gauss_solve(int n, double A[], double x[], double b[]);
    void polyfit(int n, double *x, double *y, int poly_n, double p[]);
    double getCost();
    vector<Line> getLines();
    double tmp;
};

#endif 
