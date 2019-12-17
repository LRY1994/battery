#include "optimize.h"
#include "spline.h"
#include "var.h"
using namespace std;

// void polyfit(int n, double *x, double *y, int poly_n, double p[]);
// void gauss_solve(int n, double A[], double x[], double b[]);
/*============================================================*/
////	高斯消元法计算得到	n 次多项式的系数
////	n: 系数的个数
////	ata: 线性矩阵
////	sumxy: 线性方程组的Y值
////	p: 返回拟合的结果
/*============================================================*/
void Optimize::gauss_solve(int n, double A[], double x[], double b[])
{
    int i, j, k, r;
    double max;
    for (k = 0; k < n - 1; k++)
    {
        max = fabs(A[k * n + k]); // find maxmum
        r = k;
        for (i = k + 1; i < n - 1; i++)
        {
            if (max < fabs(A[i * n + i]))
            {
                max = fabs(A[i * n + i]);
                r = i;
            }
        }
        if (r != k)
        {
            for (i = 0; i < n; i++) //change array:A[k]&A[r]
            {
                max = A[k * n + i];
                A[k * n + i] = A[r * n + i];
                A[r * n + i] = max;
            }
            max = b[k]; //change array:b[k]&b[r]
            b[k] = b[r];
            b[r] = max;
        }

        for (i = k + 1; i < n; i++)
        {
            for (j = k + 1; j < n; j++)
                A[i * n + j] -= A[i * n + k] * A[k * n + j] / A[k * n + k];
            b[i] -= A[i * n + k] * b[k] / A[k * n + k];
        }
    }

    for (i = n - 1; i >= 0; x[i] /= A[i * n + i], i--)
    {
        for (j = i + 1, x[i] = b[i]; j < n; j++)
            x[i] -= A[i * n + j] * x[j];
    }
}
/*==================polyfit(n,x,y,poly_n,a)===================*/
/*=======拟合y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n========*/
/*=====n是数据个数 xy是数据值 poly_n是多项式的项数======*/
/*===返回a0,a1,a2,……a[poly_n]，系数比项数多一（常数项）=====*/
void Optimize:: polyfit(int n, double x[], double y[], int poly_n, double p[])
{
    int i, j;
    double *tempx, *tempy, *sumxx, *sumxy, *ata;

    tempx = (double *)calloc(n, sizeof(double));
    sumxx = (double *)calloc((poly_n * 2 + 1), sizeof(double));
    tempy = (double *)calloc(n, sizeof(double));
    sumxy = (double *)calloc((poly_n + 1), sizeof(double));
    ata = (double *)calloc((poly_n + 1) * (poly_n + 1), sizeof(double));
    for (i = 0; i < n; i++)
    {
        tempx[i] = 1;
        tempy[i] = y[i];
    }
    for (i = 0; i < 2 * poly_n + 1; i++)
    {
        for (sumxx[i] = 0, j = 0; j < n; j++)
        {
            sumxx[i] += tempx[j];
            tempx[j] *= x[j];
        }
    }
    for (i = 0; i < poly_n + 1; i++)
    {
        for (sumxy[i] = 0, j = 0; j < n; j++)
        {
            sumxy[i] += tempy[j];
            tempy[j] *= x[j];
        }
    }
    for (i = 0; i < poly_n + 1; i++)
    {
        for (j = 0; j < poly_n + 1; j++)
        {
            ata[i * (poly_n + 1) + j] = sumxx[i + j];
        }
    }
    gauss_solve(poly_n + 1, ata, p, sumxy);

    free(tempx);
    free(sumxx);
    free(tempy);
    free(sumxy);
    free(ata);
}

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
    vector<Line> line;
    Line first_line; //前半段
    double min_cost = 9999;
    double xx[3] = {0, pointX[j] - pointX[i], pointX[k] - pointX[i]};
    double yy[3] = {pointY[i], pointY[j], pointY[k]};
    if (i > 0)
    {
        yy[0] = tmp;
        yy[1] = pointY[j];
        yy[2] = pointY[k];
    }

    double last_soc = 1;                                             //初始化soc
    if (opt_lines.size() > 0) {last_soc = opt_lines.back().last_soc;}//更新soc
    // cout << "soc: " << last_soc << endl;
    //first line
    line = fitting_three(xx, yy, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
        tmp = yy[1];
    }
    //second line
    yy[1] = pointY[j] - 0.3;
    line = fitting_three(xx, yy, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
        tmp = yy[1];
    }
    //third line
    yy[1] = pointY[j] + 0.3;
    line = fitting_three(xx, yy, i, last_soc);
    if (min_cost > line[0].cost + line[1].cost)
    {
        min_cost = line[0].cost + line[1].cost;
        first_line = line[0];
        tmp = yy[1];
    }
    //store the best line
    if (i + 3 == pointX.size()){
        opt_lines.push_back(first_line);
        opt_lines.push_back(line[1]);
        opt_min_cost += line[0].cost + line[1].cost;
        // write << opt_min_cost << endl;
        for (int i = 0; i < first_line.y.size(); i++)
            write << first_line.y[i] << endl;
        for (int i = 0; i < line[1].y.size(); i++)
            write << line[1].y[i] << endl;
    }
    else
    {
        opt_lines.push_back(first_line);
        opt_min_cost += line[0].cost;
        // write << opt_min_cost << endl;
        for (int i = 0; i < line[0].y.size(); i++)
            write << line[0].y[i] << endl;
    }
}

vector<Line> Optimize::fitting_three(double xx[], double yy[], int first_current_index, double last_soc)
{
    double P[3];
    int dimension = 2; //2次多项式拟合

    // int sizenum = sizeof(xx) / sizeof(xx[0]); //	拟合数据的维数
    polyfit(3, xx, yy, dimension, P);


    double  cost = 0;
    double _last_soc = last_soc;
    vector<Line> line2;
    vector<double> y;

    //两段曲线的插值
    for (int i = 0; i < 2; i++)
    {
        y.clear();
        //_last_soc、 使用引用,进行迭代
        cost = cal_line_cost(
            P[0], P[1], P[2],
            xx[i],
            xx[i + 1],
            first_current_index + i, //电流区间
            _last_soc,
            y);
        line2.push_back(Line(P[0], P[1], P[2], cost, _last_soc, y));
    }
        // cout << "cost: " << cost << endl;
    return line2;
}
//cal cost
double Optimize::cal_line_cost(
    double a, double b, double c,
    double from_t,
    double to_t,
    int current_index,
    double &_last_soc,
   vector<double>& yy)
{
    double L = 0;
    double dist = to_t - from_t;
   
    // cout << dist << endl;
    for (int step = 0; step < dist; step++)
    {
        double hh = step;
        double childy = a + b * (hh + 1) + c * pow(hh + 1, 2);
        double parenty = a + b * hh + c * hh * hh;
        yy.push_back(parenty) ;
        // write <<parenty << endl;
        L += g_ComputeObj->cal_opti_cost(parenty, childy, _last_soc, current_index);
        _last_soc = g_ComputeObj->get_opti_Soc(parenty, childy, _last_soc, current_index);
        // write << "L: " << L << endl;
    }
    // cout << "L: " << L << endl;
    // cout << "SOC:" << last_soc << endl;
    return L;
}

double Optimize::getCost()
{
    return opt_min_cost;
}
vector<Line> Optimize::getLines(){
    return opt_lines;
}

