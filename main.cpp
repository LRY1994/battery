#include "exec.h"
#include "compute.h"
#include <cstdio>
using namespace std;

int main (){
     
    Exec e(8,-20,5);// 时间间隔数；初始温度；第一层分裂数

    e.getMArray();   
    int size = e.mArray.size();   
    for (int i = 0;i < size;i++)  
    {  	
        printf("m%d=%lf\n", i, e.getM(i)); 
    }

    printf("points are:\n");
    for (int i = 0; i < e.point.size(); i++)
    {   
        printf("(%.0lf,%.2lf)   ", e.getX(i),e. getY(i));
    }
    getchar();

   return 0;
 
};
// g++  main.cpp exec.cpp tree.cpp compute.cpp spline.cpp Lookuptable.cpp Lookuptable_data.cpp