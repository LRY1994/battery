#include "spline.h"
#include<iostream>
#include<cstdio>
#include<cmath>
#include "var.h"
using namespace std;

vector<double> Spline::interpolation(vector<Point> pointList,int option)
{
        n = pointList.size()-1;
        for (int i = 0; i <= n; i++) {
            x[i] = pointList[i].x;
            y[i] = pointList[i].y;
        }
        for (int i = 0; i<n; i++){
            h[i] = x[i + 1] - x[i];
        }

        int op;
        // printf("第一种边界条件选择1，第二种边界条件选择2:");
        // scanf("%d", &op);

        if (option == 1){
            printf("输入m[0],m[n]:");
            scanf("%lf%lf", &m[0], &m[n]);

            afa[0] = 0,
            bta[0] = 2 * m[0];

            afa[n] = 1,
            bta[n] = 2 * m[n];
        }
        if (option == 2){
            afa[0] = 1,
            bta[0] = 3 * double(y[1] - y[0]) / (h[0]);

            afa[n] = 0,
            bta[n] = 3 * double(y[n] - y[n - 1]) / (h[n - 1]);
        }


        for (int i = 1; i<n; i++)
        {
            afa[i] = double(h[i - 1]) / double(h[i - 1] + h[i]);
            bta[i] = 3 * ((1 - afa[i])*double(y[i] - y[i - 1]) / double(h[i - 1]) + afa[i] * double(y[i + 1] - y[i]) / double(h[i]));
        }

        a[0] = -afa[0] / 2; b[0] = bta[0] / 2;

        

        for (int i = 1; i<n + 1; i++)
        {
            double temp = 2 + (1 - afa[i])*a[i - 1];
            a[i] = -afa[i] / temp;
            b[i] = (bta[i] - (1 - afa[i])*b[i - 1]) / temp;
        }
        m[n + 1] = 0;

        // printf("解得\n");
        for (int i = n; i >= 0; i--)
        {
            m[i] = a[i] * m[i + 1] + b[i];
            // printf("m%d=%lf\n", i, m[i]);
        }
       
        vector<double> mVector;  
        for(int i=0;i<=n;i++){
            mVector.push_back(m[i]);
        }
   
        return mVector;

}

double Spline::getValue(double xx){
    int from=-1, to=-1;
    for (int i = 0; i <= n; i++){
        // if (i == 0 && xx <= double(x[i + 1] )&& xx >= double(x[i])){
        // 	from = i; to = i + 1;
        // 	break;
        // }
        // if (i>0 && xx <= double(x[i + 1]) && xx>double(x[i])){
        // 	from = i; to = i + 1;
        // 	break;
        // }
        if (xx <= x[i + 1] && xx >= x[i]){
            from = i; to = i + 1;
            break;
        }
    }
    if(from==-1) return -10;

    double a1 = (xx - x[from]) / (x[to] - x[from]);
    double a2 = (xx - x[to]) / (x[from] - x[to]);
    double b1 = a1*a1;
    double b2 = a2*a2;
    double result =   (1 + 2 * a1) * b2 * y[from]
                    + (1 + 2 * a2) * b1 * y[to]
                    + (xx - x[from]) * b2 * m[from]
                    + (xx - x[to]) * b1 * m[to];

    return result;

}
