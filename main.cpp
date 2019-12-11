#include "optimize.h"
#include "compute.h"
#include "exec.h"
#include "manual.h"
#include "var.h"
#include <cstdio>
#include <iostream>
#include <stdio.h>
using namespace std;

/*** input **/
//mode==1
char *M_DATA_FILE = (char*)"current_data\\24hours.txt"; ;
int M_FILE_FROM_POS =0;
int M_FILE_TO_POS =86400;
double M_FROM_T = 0; //initial temp
double M_TO_T = -20;//target temp
//mode==2 
 char *DATA_FILE = (char*)"current_data\\24hours.txt";
 int FILE_FROM_POS = 0;
 int FILE_TO_POS = 86400;
 double FROM_T = 0;//初始温度；


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
            cout << "the cost of manual operation is " << endl << M.getCost() << endl;
            printf("choose the mode again(1 for manual mode, 2 for intelligent mode):  ");
        }
        /**************** 智能 ***********************/
        else if (mode == 2)
        {
            Exec E( DATA_FILE, 
                    FILE_FROM_POS,  
                    FILE_TO_POS ,
                    FROM_T
                ); 
   

            printf("\nall min cost is %lf:\n", E.getCost());
            write << endl << "all min cost is:" << E.getCost() << endl;
            printf("points are( time , temperature ):\n");
            for (int i = 0; i < E.getPointSize(); i++) printf(" %.2lf  ,  %.2lf  \n", E.getX(i), E.getY(i));
            printf("choose the mode again(1 for manual mode, 2 for intelligent mode):  ");

            //optimize
            Optimize OPT( E.pointX, E.pointY);
            printf("\nafter optimize min cost is %lf:\n", OPT.getCost());
      
        }

        else
        {
            printf("error");
        }
    }
    getchar();
    return 0;
};
// g++  main.cpp exec.cpp tree.cpp compute.cpp spline.cpp simulink/Lookuptable.cpp simulink/Lookuptable_data.cpp current.cpp manual.cpp optimize.cpp