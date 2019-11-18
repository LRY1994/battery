#include "compute.h"
#include "exec.h"
#include "manual.h"
#include <cstdio>
#include <iostream>
#include "var.h"
#include <stdio.h>
using namespace std;

/*** input **/
//mode==1
char *M_DATA_FILE = (char*)"current_data\\UDDS.txt"; ;
int M_FILE_FROM_POS =0;
int M_FILE_TO_POS =300;
double M_FROM_T = 0; //initial temp
double M_TO_T = 30;//target temp
//mode==2 
 char *DATA_FILE = (char*)"current_data\\UDDS.txt";
 int FILE_FROM_POS = 0;
 int FILE_TO_POS = 300;
 double INITIAL_T = 0;//初始温度；
 int FIRST_LAYER_SPLIT = 5;//第一层分裂数;
 int SPLIT = 4;//其它层分裂数


/*** global variable  **/
Compute *g_ComputeObj=nullptr;
vector<Current_Area> g_CurrentData;
ofstream write;

int main (){
    // set the mode;
    int mode;
    printf("choose the mode (1 for manual mode, 2 for intelligent mode):  ");

    while (scanf("%d", &mode) != EOF){
         /**************** 手动 ***********************/
         if (mode == 1)
         {
            Manual M (  M_DATA_FILE, 
                        M_FILE_FROM_POS,  
                        M_FILE_TO_POS ,
                        M_FROM_T,
                        M_TO_T
                    ); 
            double cost = M.getCost();
            cout << "the cost of manual operation is " << cost << endl;
            printf("choose the mode again(1 for manual mode, 2 for intelligent mode):  ");
        }
        /**************** 智能 ***********************/
        else if (mode == 2)
        {
            Exec e( DATA_FILE, 
                    FILE_FROM_POS,  
                    FILE_TO_POS ,
                    INITIAL_T,
                    FIRST_LAYER_SPLIT,
                    SPLIT
                ); //初始温度；第一层分裂数;其它层分裂数
   

            printf("\nall min cost is %.2lf:\n", e.getCost());
            printf("points are( time , temperature ):\n");
            for (int i = 0; i < e.getPointSize(); i++) printf("( %.2lf  ,  %.2lf ) \n", e.getX(i), e.getY(i));
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