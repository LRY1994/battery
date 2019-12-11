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
    double d;
    double last_soc;
    double cost;

    Line(double aa, double bb, double cc, double dd, double ccost, double last_socc)
    {
        a = aa;
        b = bb;
        c = cc;
        d = dd;
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
    vector<Line> spline_three(vector<Point> pointList, int first_current_index, int last_soc);
    double cal_line_cost(
        double a, double b, double c, double d,
        double from_t,
        double to_t,
        int current_index,
        double &last_soc);

  public:
    Optimize(vector<double> pointX, vector<double> pointY);
    double getCost();
    vector<Line> getLines();
};

#endif 
