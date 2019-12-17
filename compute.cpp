#include <iostream>
#include <vector>
#include "simulink/Lookuptable.h"
#include "simulink/rtwtypes.h"
#include "var.h"
#include "compute.h"
#include "math.h"
using namespace std;


Compute::Compute(vector<Current_Area> current){
    rtObj = new LookuptableModelClass(); 
    rtObj->initialize();
    currentData = current;
}

/***************************************private*********************************/

// Input I algorithm
double Compute:: getI(int layer){ 
    double I = currentData[layer].current;
    return I;
}

double Compute:: getDt(int layer){
    return currentData[layer].dt;
}

double Compute:: getDegree(int layer){
    return currentData[layer].degree;
}

double Compute::getTime(int layer){
    double result = 0;
    for (int i = 0; i <= layer;i++){
        result += currentData[i].dt;
    }
    return result;
}

// Qt
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
    double R0 = 12 * 0.001 * rtObj->rtY.R0;
    return R0;
}

//cooling power
double Compute::getPcool(double T, double Tnex,int layer) {
    double Tenv = getTenv(layer);
    return ((Tnex + T) / 2 - Tenv) / (deltax / (lamda * A) + 1 / (ha * A));
}

double Compute::getTenv(int layer){
    double time = getTime(layer);
    double Tenv;
    if (time <= 3600)
    {
        Tenv = (Tenv1[0] + Tenv1[1]) / 2;
    }
    else if (time > 3600 && time <= 7200)
    {
        Tenv = (Tenv1[1] + Tenv1[2]) / 2;
    }
    else if (time > 7200 && time <= 10800)
    {
        Tenv = (Tenv1[2] + Tenv1[3]) / 2;
    }
    else if (time > 10800 && time <= 14400)
    {
        Tenv = (Tenv1[3] + Tenv1[4]) / 2;
    }
    else if (time > 14400 && time <= 18000)
    {
        Tenv = (Tenv1[4] + Tenv1[5]) / 2;
    }
    else if (time > 18000 && time <= 21600)
    {
        Tenv = (Tenv1[5] + Tenv1[6]) / 2;
    }
    else if (time > 21600 && time <= 25200)
    {
        Tenv = (Tenv1[6] + Tenv1[7]) / 2;
    }
    else if (time > 25200 && time <= 28800)
    {
        Tenv = (Tenv1[7] + Tenv1[8]) / 2;
    }
    else if (time > 28800 && time <= 32400)
    {
        Tenv = (Tenv1[8] + Tenv1[9]) / 2;
    }
    else if (time > 32400 && time <= 36000)
    {
        Tenv = (Tenv1[9] + Tenv1[10]) / 2;
    }
    else if (time > 36000 && time <= 39600)
    {
        Tenv = (Tenv1[10] + Tenv1[11]) / 2;
    }
    else if (time > 39600 && time <= 43200)
    {
        Tenv = (Tenv1[11] + Tenv1[12]) / 2;
    }
    else if (time > 43200 && time <= 46800)
    {
        Tenv = (Tenv1[12] + Tenv1[13]) / 2;
    }
    else if (time > 46800 && time <= 50400)
    {
        Tenv = (Tenv1[13] + Tenv1[14]) / 2;
    }
    else if (time > 50400 && time <= 54000)
    {
        Tenv = (Tenv1[14] + Tenv1[15]) / 2;
    }
    else if (time > 54000 && time <= 57600)
    {
        Tenv = (Tenv1[15] + Tenv1[16]) / 2;
    }
    else if (time > 57600 && time <= 61200)
    {
        Tenv = (Tenv1[16] + Tenv1[17]) / 2;
    }
    else if (time > 61200 && time <= 64800)
    {
        Tenv = (Tenv1[17] + Tenv1[18]) / 2;
    }
    else if (time > 64800 && time <= 68400)
    {
        Tenv = (Tenv1[18] + Tenv1[19]) / 2;
    }
    else if (time > 68400 && time <= 72000)
    {
        Tenv = (Tenv1[19] + Tenv1[20]) / 2;
    }
    else if (time > 72000 && time <= 75600)
    {
        Tenv = (Tenv1[20] + Tenv1[21]) / 2;
    }
    else if (time > 75600 && time <= 79200)
    {
        Tenv = (Tenv1[21] + Tenv1[22]) / 2;
    }
    else if (time > 79200 && time <= 82800)
    {
        Tenv = (Tenv1[22] + Tenv1[23]) / 2;
    }
    else if (time > 82800 && time <= 86400)
    {
        Tenv = (Tenv1[23] + Tenv1[24]) / 2;
    }
    return Tenv;
}


//exo power
double Compute::getPexo(double T,double I,double SOC) {
    double R0 = getR(T, SOC);
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
    return (Pptc * eta - Pcool + Pexo) * dt / Ctotal;
}

//delta SOC
double Compute::getDeltaSoc(double parentT, double childT, double parentSOC, int parentLayer, double I)
{
    int dt = getDt(parentLayer);
    double Pcool = getPcool(parentT, childT, parentLayer);
    double Pexo = getPexo(parentT,I,parentSOC);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,dt);
    double Qt = getQt(I,childT);
    double dsoc = ( I + Pptc / Uptc ) * ( dt / 3600) / Qt;
    return  dsoc;
}



/******************* public *************************/

double Compute::get_max_T(double parentT, double parentSOC, int parentLayer)
{
    double Pptc = Pmax;
    double I = getI(parentLayer);
    int dt = getDt(parentLayer);
    double Pcool = getPcool(parentT, parentT, parentLayer);
    double Pexo = getPexo(parentT,I,parentSOC);
    return getDeltaT(Pptc,Pcool,Pexo,dt) + parentT;

}

double Compute::get_min_T(double parentT, double parentSOC, int parentLayer)
{
    double Pptc = 0;
    double I = getI(parentLayer);
    int dt = getDt(parentLayer);
    double Pcool = getPcool(parentT, parentT, parentLayer);
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
// double Compute:: get_firstLayer_T(int i,int N,double parentT,double SOC){
//     const int layer = 0;
//     double I = getI(layer);
//     double Tmin = get_min_T(parentT,layer,SOC);
//     double Tmax = get_max_T(parentT,layer,SOC);
//     double dist = Tmax - Tmin;
//     double seg = dist / N;
//     double childT = Tmin + i * seg; //i从1开始计算，温度从小到大排序
//     cout << "herererere" << endl;
//     return childT;

// }
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
    double Pcool = getPcool(parentT, childT, parentLayer);
    double Pexo = getPexo(parentT,I, parentSOC);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,dt);
    double Ah_cost = getCost(Pptc, Qt, I, dt);
    double Cyc_cost;
    double Cal_cost;
    double fenmu = R_gas * (abs(25 - parentT) + 248.15);
    double fenzi = (-Eacyc + alpha * abs(I + Pptc/Uptc) / 37);
    if (Ah_cost >= 0)
    {
        Cyc_cost = Bcyc * exp(fenzi / fenmu) * pow(Ah_cost, Zcyc);
        Cal_cost = Bcal * exp(-Eacal / fenmu) * pow(dt, Zcal);
    }
    else
    {
        Cyc_cost = Bcyc * exp(fenzi / fenmu);
        Cal_cost = Bcal * exp(-Eacal / fenmu) * pow(dt, Zcal);
    }
    double all_cost;
    if (abs(I) > 3)
    {
        all_cost = money_perAh * Ah_cost + money_perSOH * Cyc_cost;
        // cout << "Ah_cost: " << Ah_cost << "Cyc_cost: " << Cyc_cost << endl;
    }
    else
    {
        all_cost = money_perAh * Ah_cost + money_perSOH * Cal_cost;
        // cout << "Ah_cost: " << Ah_cost << "Cyc_cost: " << Cal_cost << endl;
    }
    return all_cost;
}

double Compute::cal_opti_cost(double parentT, double childT, double parentSOC, int parentLayer)
{
    int dt = 1;
    double I = getI(parentLayer);
    double Qt = getQt(I, childT);
    double Pcool = getPcool(parentT, childT, parentLayer);
    double Pexo = getPexo(parentT, I, parentSOC);
    double Pptc = getPptc(parentT, childT, Pcool, Pexo, dt);
    double Ah_cost = getCost(Pptc, Qt, I, dt);
    double Cyc_cost;
    double Cal_cost;
    double fenmu = R_gas * (abs(25 - parentT) + 248.15);
    double fenzi = (-Eacyc + alpha * abs(I + Pptc / Uptc) / 37);
    if (Ah_cost >= 0)
    {
        Cyc_cost = Bcyc * exp(fenzi / fenmu) * pow(Ah_cost, Zcyc);
        Cal_cost = Bcal * exp(-Eacal / fenmu) * pow(dt, Zcal);
    }
    else
    {
        Cyc_cost = Bcyc * exp(fenzi / fenmu);
        Cal_cost = Bcal * exp(-Eacal /fenmu) * pow(dt, Zcal);
    }
    double all_cost;
    if (abs(I) > 3)
    {
        all_cost = money_perAh * Ah_cost + money_perSOH * Cyc_cost;
    }
    else
    {
        all_cost = money_perAh * Ah_cost + money_perSOH * Cal_cost;
    }
    return all_cost;
}

double Compute::getCost(double Pptc, double Qt, double I, int dt)
    {
        if (Pptc > 0)
        {
            return ((Pptc + Ppump) / Uptc + I) * Q0 * dt / Qt / 3600;
        }
        else
        {
            return I * Q0 * dt / Qt / 3600;
        }
    }

double Compute::getSoc(double parentT, double childT, double parentSOC, int parentLayer)
    {
        // int dt = getDt(parentLayer);
        double I = getI(parentLayer);
        double curSOC = parentSOC - getDeltaSoc(parentT, childT, parentSOC, parentLayer, I);
        if (curSOC >= 0)
        {
            return curSOC;
        }
        else
        {
            printf("No power!");
            abort();
        }
    }

double Compute::get_opti_Soc(double parentT, double childT, double parentSOC, int parentLayer)
    {
        double I = getI(parentLayer);
        int dt = 1;
        double Pcool = getPcool(parentT, childT, parentLayer);
        double Pexo = getPexo(parentT, I, parentSOC);
        double Pptc = getPptc(parentT, childT, Pcool, Pexo, dt);
        double Qt = getQt(I, childT);
        double dsoc = (I + Pptc / Uptc)  / (3600* Qt);
        // write << "i:" << I << ";Pptc:" << Pptc << ";Uptc:" << Uptc << ";Qt" << Qt << ";Pcool:" << Pcool << ";Pexo:" << Pcool << "parentT:" << parentT << "childT:" << childT << "dsoc:" << dsoc << endl;
        // cout << "Pptc:" << Pptc<<endl;
        // if (dsoc > 0)
        // {
        //     cout << dsoc << endl;
        // }
        
        double curSOC = parentSOC - dsoc;
        // write << dsoc << endl;
        if (curSOC >= 0)
        {
            return curSOC;
        }
        else
        {
            printf("No power!");
            abort();
        }
    }