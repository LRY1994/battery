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
#include <stdio.h>
using namespace std;

Compute *MComputeObj = nullptr;
int Mlayer;
ofstream write2;


Manual::Manual( char *file, int fromPos, int toPos ,double a_fromT, double a_toT )
{
    fromT = a_fromT;
    toT = a_toT;  

    //handle current data
    Current *CurrentObj = new Current(file, fromPos, toPos);
    vector<Current_Area> current_data = CurrentObj->getpoint();

    Mlayer = current_data.size();
    cout << "size is " << Mlayer << endl;
    //initilize Compute
    MComputeObj = new Compute(current_data);
    write2.open("log_manual.txt", ios::out);
}

double Manual::getCost(){
    double T = fromT;
    double Ah_cost = 0;
    double SOC = 1;

   
    double time = 0;
    double I;
    double Qt;
    double Pcool;
    double Pexo;

    double DeltaT;
    double parentT;

    double DeltaSOC;
   
    double Pptc;
    double PrePTC = 0;
    static constexpr double alpha = 1243.33;
    static constexpr double Bcyc = 0.0039; 
    static constexpr double Bcal = 56128; //ok
    static constexpr double Eacyc = 16361; //
    static constexpr double Eacal = 62862; //ok
    static constexpr double Zcyc = 0.5644; //ok
    static constexpr double Zcal = 0.4973; //ok
    static constexpr double money_perAh = 0.0864; //ok
    static constexpr double money_perSOH = 15096; //ok
    static constexpr double R_gas = 8.314; //ok
    static constexpr double Uptc = 48;

    double total_cost = 0;

    FILE *fpWrite;

    for (int i = 0; i < Mlayer; i=i+1)
    {
        // int dt = MComputeObj->getDt(i);
        int dt = 1;
        if (T < toT - 1)
        {
             Pptc = 500;
             PrePTC = 500;
        }
        else if (T > toT - 1 && T < toT)
        {
            if (PrePTC==500){
                Pptc = 500;
                PrePTC = 500;
            }
            else
            {
                Pptc = 0;
                PrePTC = 0;
            }   
        }
        else
        {
            Pptc = 0;
            PrePTC = 0;
        }
        //  time = MComputeObj->getTime(i);        
         I = MComputeObj->getI(i);
         Qt = MComputeObj->getQt(I, T);
         Pcool = MComputeObj->getPcool(T, T, i);
         Pexo = MComputeObj->getPexo(T, I, SOC);
         Ah_cost = MComputeObj->getCost(Pptc, Qt, I, dt);
        //  cout << Ah_cost << endl;
         DeltaT = MComputeObj->getDeltaT(Pptc, Pcool, Pexo, dt);
         parentT = T;
         //  cout << T << endl;
         T += DeltaT;
         double Cyc_cost;
         double Cal_cost;
         if (Ah_cost >= 0)
         {
             Cyc_cost = Bcyc * exp((-Eacyc + alpha * abs((I + Pptc / Uptc)) / 37) / R_gas / (abs(25 - parentT) + 248.15)) * pow(Ah_cost, Zcyc);
             Cal_cost = Bcal * exp(-Eacal / R_gas / (abs(25 - parentT) + 248.15)) * pow(dt, Zcal);
         }
         else
         {
             Cyc_cost = Bcyc * exp((-Eacyc + alpha * abs((I + Pptc / Uptc)) / 37) / R_gas / (abs(25 - parentT) + 248.15));
             Cal_cost = Bcal * exp(-Eacal / R_gas / (abs(25 - parentT) + 248.15)) * pow(dt, Zcal);
         }

         double all_cost;

         if (abs(I) > 3)
         {
             all_cost = money_perAh * Ah_cost + money_perSOH * Cyc_cost;
            //  write2 << Ah_cost + Cyc_cost << endl;
         }
         else
         {
             all_cost = money_perAh * Ah_cost + money_perSOH * Cal_cost;
            //  write2 << Ah_cost + Cal_cost << endl;
         }
         DeltaSOC = MComputeObj->getDeltaSoc(parentT, T, SOC, dt, I);
         SOC = SOC - DeltaSOC;
        //  write2 << T << endl;
         total_cost += all_cost;
         write2 << total_cost << endl;
    }
    return total_cost;
}