#include "compute.h"
#include "exec.h"
#include "manual.h"
#include <cstdio>
#include <iostream>
#include <stdio.h>

using namespace std;

int main (){

    // set the mode;
    int mode;
    printf("choose the mode (1 for manual mode, 2 for intelligent mode):  ");
    // scanf("%d", &mode);
    // getchar();
    while (scanf("%d", &mode) != EOF){

        if (mode == 1)
        {

            /**************** 手动 ***********************/

            Manual M(0, 30); //fromT\toT
            double cost = M.getCost();
            cout << "the cost of manual operation is " << cost << endl;
            printf("choose the mode again(1 for manual mode, 2 for intelligent mode):  ");
        }

        else if (mode == 2)
        {

            /**************** 智能 ***********************/

            Exec e(0, 5, 4); //初始温度；第一层分裂数;其它层分裂数

            e.buildMultiTree();
            e.makePoints();
            // int size = e.mArray.size();
            // for (int i = 0; i < size; i++)
            // {
            //     printf("m%d=%lf\n", i, e.getM(i));
            // }

            printf("\nall min cost is %.2lf:\n", e.getAllCost());

            printf("points are( time , temperature ):\n");
            for (int i = 0; i < e.point.size(); i++)
            {
                printf("( %.2lf  ,  %.2lf ) \n", e.getX(i), e.getY(i));
            }
            printf("choose the mode again(1 for manual mode, 2 for intelligent mode):  ");
        }

        else
        {
            printf("error");
        }
    }
    getchar();
    return 0;
};
// g++  main.cpp exec.cpp tree.cpp compute.cpp spline.cpp simulink/Lookuptable.cpp simulink/Lookuptable_data.cpp current.cpp manual.cpp