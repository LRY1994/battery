#include <stdio.h>
#include <stdlib.h>

#include "math.h"

void polyfit(int n, double *x, double *y, int poly_n, double p[]);
void gauss_solve(int n, double A[], double x[], double b[]);

/*==================polyfit(n,x,y,poly_n,a)===================*/
/*=======拟合y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n========*/
/*=====n是数据个数 xy是数据值 poly_n是多项式的项数======*/
/*===返回a0,a1,a2,……a[poly_n]，系数比项数多一（常数项）=====*/
void polyfit(int n, double x[], double y[], int poly_n, double p[])
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
/*============================================================*/
////	高斯消元法计算得到	n 次多项式的系数
////	n: 系数的个数
////	ata: 线性矩阵
////	sumxy: 线性方程组的Y值
////	p: 返回拟合的结果
/*============================================================*/
void gauss_solve(int n, double A[], double x[], double b[])
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
void main()
{
    int i, sizenum;
    double P[6];
    int dimension = 5; //5次多项式拟合
    //	要拟合的数据
    double xx[] = {0.995119, 2.001185, 2.999068, 4.001035, 4.999859, 6.004461, 6.999335,
                   7.999433, 9.002257, 10.003888, 11.004076, 12.001602, 13.003390, 14.001623, 15.003034,
                   16.002561, 17.003010, 18.003897, 19.002563, 20.003530};
    double yy[] = {-7.620000, -2.460000, 108.760000, 625.020000, 2170.500000, 5814.580000,
                   13191.840000, 26622.060000, 49230.220000, 85066.500000, 139226.280000, 217970.140000, 328843.860000,
                   480798.420000, 684310.000000, 951499.980000, 1296254.940000, 1734346.660000, 2283552.120000, 2963773.500000};

    sizenum = sizeof(xx) / sizeof(xx[0]); //	拟合数据的维数
    polyfit(sizenum, xx, yy, dimension, P);

    printf("拟合系数, 按升序排列如下:\n");
    for (i = 0; i < dimension + 1; i++) //这里是升序排列，Matlab是降序排列
    {
        printf("P[%d]=%lf\n", i, P[i]);
    }
}