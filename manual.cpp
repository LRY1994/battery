/*
* 对照组，手动模式
*/
#include "current.h"
#include "manual.h"
#include "spline.h"
#include "tree.h"
#include "var.h"
#include <cstdio>
#include <fstream>
using namespace std;

Compute *MComputeObj = nullptr;
int Mlayer;

Manual::Manual(double a_fromT, int a_toT)
{
    fromT = a_fromT;
    toT = a_toT;  

    //handle current data
    Current *CurrentObj = new Current();
    char *p = (char *)"current_data\\NEDC.txt";
    vector<Point> data = CurrentObj->readData(p, 0, 1370);
    vector<Current_Area> current_data = CurrentObj->processData(data);

    Mlayer = current_data.size();

    //initilize Compute
    MComputeObj = new Compute(current_data);

}

double Manual::getCost(){
    double T = fromT;
    double cost = 0;
    double SOC = 1;

   
    double time = 0;
    double I  ;
    double Qt;
    double Pcool ;
    double Pexo ;
   
    double DeltaT;
    double parentT ;

    double DeltaSOC;
   
    double Pptc;

    for (int i = 0; i < Mlayer; i++)
    {
        int dt = MComputeObj->getDt(i);
        if (T < toT)
        {
             Pptc = 500;
            
        }
        else
        {
            Pptc = 0;
        }

         time = MComputeObj->getTime(time, i);        

         I = MComputeObj->getI(i);
         Qt = MComputeObj->getQt(I, T);
         Pcool = MComputeObj->getPcool(T, T);
         Pexo = MComputeObj->getPexo(T, I, SOC);
        cost += MComputeObj->getCost(Pptc, Qt, I, dt);
         DeltaT = MComputeObj->getDeltaT(Pptc, Pcool, Pexo, dt);
         parentT = T;
        cout << "(" << time << "," << T << ")" << endl;
        T += DeltaT;
         DeltaSOC = MComputeObj->getDeltaSoc(parentT, T, SOC, dt, I);
         SOC = SOC - DeltaSOC;
    }
    return cost;
}