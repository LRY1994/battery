#include "optimize.h"
#include "spline.h"
#include "var.h"
using namespace std;

Optimize::Optimize(vector<double> pointXX, vector<double> pointYY)
{
    pointX = pointXX;
    pointY = pointYY;
    opt_lines.clear();
    opt_min_cost = 0;
   
    for (int i = 0; i + 2 < pointX.size(); i++)
    {
        three(i, i + 1, i + 2);
    }


}

void Optimize::three(double i, double j, double k)
{
    vector<Point> pointList;
    vector<Line> line;
    Line first_line; //前半段
    double min_cost = 9999;

    pointList.push_back(Point(pointX[i], pointY[i]));
    pointList.push_back(Point(pointX[k], pointY[k]));
    pointList.push_back(Point(pointX[j], pointY[j]));

    double last_soc = 0;
    if (opt_lines.size() > 0) last_soc = opt_lines.back().last_soc;
    //first line
    line = spline_three(pointList, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
    }
    //second line
    pointList.pop_back();
    pointList.push_back(Point(pointX[j], pointY[j] + 0.5));
    line = spline_three(pointList, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
    }
    //third line
    pointList.pop_back();
    pointList.push_back(Point(pointX[j], pointY[j] - 0.5));
    line = spline_three(pointList, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
    }
    //store the best line
    opt_lines.push_back(first_line);
    opt_min_cost += line[0].cost;
}

vector<Line> Optimize::spline_three(vector<Point> pointList,int first_current_index,int last_soc)
{
    Spline *s = new Spline();
    vector<double> mVector = s->interpolation(pointList, 2);

    double a, b, c, d, cost=0;  
    double _last_soc = last_soc;
    vector<Line> line;

    //两段曲线的插值
    for (int i = 0; i < 2; i++)
    {
        double h = pointList[i + 1].x - pointList[i].x;
        a = pointList[0].y;
        b = (pointList[i + 1].y - pointList[i].y) / h -
            h * mVector[i] / 2 -
            h * (mVector[i + 1] - mVector[i]) / 6;
        c = mVector[i] / 2;
        d = (mVector[i + 1] - mVector[i]) / (6 * h);

        //_last_soc、 使用引用,进行迭代
        cost = cal_line_cost(
                    a, b, c, d,
                    pointList[i].x,
                    pointList[i + 1].x,
                    first_current_index + i,//电流区间
                    _last_soc
                );

        line.push_back(Line(a, b, c, d, cost, _last_soc));
    }
    return line;
}
//cal cost
double Optimize::cal_line_cost(
    double a, double b, double c, double d,
    double from_t,
    double to_t,
    int current_index,
    double &last_soc)
{
    double L = 0;
    double dist = to_t - from_t;
    for (int j = 1; j < dist; j++)
    {
        double hh = from_t + j;
        double childy = a + b * (hh - 1) + c* pow(hh - 1, 2) + d * pow(hh - 1, 3);
        double parenty = a + b * hh + c* hh * hh + d * hh * hh * hh;
        L += g_ComputeObj->cal_cost(parenty, childy, last_soc, current_index);
        last_soc = g_ComputeObj->getSoc(parenty, childy, last_soc, current_index);
    }

    return L;
}

double Optimize::getCost()
{
    return opt_min_cost;
}
vector<Line> Optimize::getLines(){
    return opt_lines;
}