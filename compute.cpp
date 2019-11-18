#include <iostream>
#include <vector>
#include "simulink/Lookuptable.h"
#include "simulink/rtwtypes.h"
#include "var.h"
#include "compute.h"
using namespace std;


Compute::Compute(vector<Current_Area> current){
    rtObj = new LookuptableModelClass(); 
    rtObj->initialize();
    g_CurrentData = current;
}

/***************************************private*********************************/

// Input I algorithm
double Compute:: getI(int layer){
    return g_CurrentData[layer].current;
}

double Compute:: getDt(int layer){
    return g_CurrentData[layer].dt;
}

double Compute::getTime(double lastTime,int layer){
    return lastTime + getDt(layer);
}

//Qt
double Compute:: getQt(double I,double Tnex){
    double Beta = I / 37;
    if (Beta < 0){
        Beta = -Beta;
    }
   
    rtObj->rtU.Tnex = Tnex;
    rtObj->rtU.Beta = Beta;
    rtObj->step();
    double Qt = Q0 * (rtObj->rtY.Qt) / 100;
    return Qt;
}

//R0
double Compute::getR(double T,double SOC){
    rtObj->rtU.T = T;
    rtObj->rtU.SOC = SOC;
    rtObj->step();
    double R0 = 0.001 * rtObj->rtY.R0;
    // write << "R0:"<<R0<<endl;
    // printf("R0:%lf\n\n",R0);
    return R0;
}

//cooling power
double Compute::getPcool(double T,double Tnex) {
    return ((Tnex+T)/2-Tenv)/(deltax/(lamda*A)+1/(ha*A));
}

//exo power
double Compute::getPexo(double T,double I,double SOC) {
    double R0 = 0.08 * getR(T, SOC);
    return I * I * R0;
}

//PTC power
double Compute::getPptc(double T,double Tnex,double Pcool,double Pexo,int dt) {
    double Pptc = ( Tnex-T ) * Ctotal / dt + Pcool - Pexo;
        if (Pptc < 0){
            return 0;
        }
        else if (Pptc > 500){
            return 500;
        }
        return Pptc;
}

//delta temperature
double Compute::getDeltaT(double Pptc,double Pcool,double Pexo,int dt) {
    return ( Pptc - Pcool + Pexo ) * dt / Ctotal;
}

//delta SOC
double Compute:: getDeltaSoc(double parentT, double childT, double parentSOC,int dt,double I){
    double Pcool = getPcool(parentT,childT);
    double Pexo = getPexo(parentT,I,parentSOC);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,dt);
    double Qt = getQt(I,childT);
    double dsoc = ( I + Pptc / Uptc ) * ( dt / 3600) / Qt;
    return  dsoc;
}



/******************* public *************************/

double Compute:: get_max_T(double parentT,double parentSOC,int parentLayer)
{
    double Pptc = Pmax;
    double I = getI(parentLayer);
    int dt = getDt(parentLayer);
    double Pcool = getPcool(parentT,parentT);
    double Pexo = getPexo(parentT,I,parentSOC);
    // cout << "I: " << I << "dt: " << dt << "Pcool: " << Pcool << "Pexo: " << Pexo;
    return getDeltaT(Pptc,Pcool,Pexo,dt) + parentT;

}

double Compute::get_min_T(double parentT,double parentSOC,int parentLayer)
{
    double Pptc = 0;
    double I = getI(parentLayer);
    int dt = getDt(parentLayer);
    double Pcool = getPcool(parentT,parentT);
    double Pexo = getPexo(parentT,I,parentSOC);
    return getDeltaT(Pptc,Pcool,Pexo,dt) + parentT;
}
/**
* define first layer temperature
* i :index of childNodes
* N :number of first layer nodes
* parentT :the temperature of parent
*
*/
double Compute:: get_firstLayer_T(int i,int N,double parentT,double SOC){
    const int layer = 0;
    double I = getI(layer);
    double Tmin = get_min_T(parentT,layer,SOC);
    double Tmax = get_max_T(parentT,layer,SOC);
    double const M = 2 * N;
    double childT = ( M - 2 * i + 1) * Tmin / M  + (2 * i - 1) * Tmax / M ;//i从1开始计算，温度从小到大排序

    // printf("parentT:%lf\n",parentT);    
    // printf("childTmin:%lf\n",Tmin);   
    // printf("childTmax:%lf\n",Tmax);
    // printf("childT:%lf\n\n", childT);

    write << "childT:" << childT << endl;
    write << "parentT:" << parentT << endl;
    write << "childTmin:" << Tmin << endl;
    write << "childTmax:" << Tmax << endl;


    return childT;

}
/**
* parentT :parent's temperature
* childT:child's temperature
* layer: parent's layer,begin with 0
*/
double Compute::cal_cost(double parentT,double childT,double parentSOC,int parentLayer)
{
    int dt = getDt(parentLayer);
    double I = getI(parentLayer);
    double Qt = getQt(I,childT);
    double Pcool = getPcool(parentT,childT);
    double Pexo = getPexo(parentT,I, parentSOC);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,dt);
    // printf("Qt:%.4lf; Pcool:%.4lf; Pexo:%.4lf; Pptc:%.4lf; I:%lf;dt:%ld\n",Qt, Pcool, Pexo, Pptc,I,dt);
    write << "Qt:"<<Qt<<",Pcool:"<<Pcool<<",Pptc:"<<Pptc<< ",I:"<<I<<",dt:"<<dt<<endl;
    // return  (Pptc/Uptc+I) * (Q0/Qt) * (dt / 3600);
    double a = getCost(Pptc, Qt, I, dt);
    // cout << "cost:" << a << endl;

    return a;

}

double Compute::getCost(double Pptc,double Qt,double I, int dt ){
    return (Pptc / Uptc + I) * Q0 * dt / Qt / 3600;
}

double Compute::getSoc(double parentT, double childT, double parentSOC, int parentLayer)
{
    int dt = getDt(parentLayer);
    double I = getI(parentLayer);
    double curSOC = parentSOC - getDeltaSoc(parentT, childT, parentSOC, dt, I);
    if (curSOC >= 0)
    {
        return curSOC;
    }
    else{
        printf("No power!");
        abort();
    }
}
