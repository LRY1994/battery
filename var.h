#include <vector>
#include "compute.h"
#include <fstream>
using namespace std;

#ifndef _VAR_H
#define _VAR_H
    /** input **/
    //mode==1
    extern  char *M_DATA_FILE ;
    extern  int M_FILE_FROM_POS ;
    extern  int M_FILE_TO_POS ;
    extern  double M_FROM_T ; //initial temp
    extern  double M_TO_T;//target temp
    //mode==2
    extern  char *DATA_FILE ;
    extern  int FILE_FROM_POS ;
    extern  int FILE_TO_POS ;
    extern  double INITIAL_T ;//初始温度；
    extern  int FIRST_LAYER_SPLIT ;//第一层分裂数;
    extern  int SPLIT ;//其它层分裂数



    /*global variable*/
    extern Compute *g_ComputeObj;
    extern ofstream write;
    extern vector<Current_Area> g_CurrentData;

#endif // _VAR_H

