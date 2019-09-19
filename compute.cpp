#include <iostream>
#include <vector>
#include "simulink/Lookuptable.h"
#include "simulink/rtwtypes.h"
#include "var.h"
#include "compute.h"
using namespace std;


Compute::Compute(int segment){
    initRtObj();
    setDt(segment);
}

/***************************************private*********************************/


void  Compute::initRtObj(){
    rtObj = new LookuptableModelClass(); 
    // rtObj->initialize();
}

// adaptive dt algorithm（可选）
void Compute:: setDt(int segment){
    for(int i=0;i<segment;i++){
        dt.push_back(10);
    }
}

// Input I algorithm（还没完成）
double Compute:: getI(){
    return 10;
}

double Compute::getTime(double lastTime,int layer){
    return lastTime + dt[layer-1];
}

//Qt(还没完成)
double Compute:: getQt(double I,double Tnex){
    double Beta = I/37;
    rtObj->initialize();
    rtObj->rtU.Tnex = Tnex;
    rtObj->rtU.Beta = Beta;
    rtObj->step();
    double Qt = 0.37*(rtObj->rtY.Qt);
    return Qt;
}

//R0
double Compute::getR(double T,double I,double SOC,int layer){
    rtObj->initialize();
    rtObj->rtU.T = T;
    rtObj->rtU.SOC = SOC;
    rtObj->step();
    double R0 = rtObj->rtY.R0;
    write << "R0:"<<R0<<endl;
    printf("R0:%lf\n\n",R0);
    return R0;
}

//cooling power
double Compute::getPcool(double T,double Tnex) {
    return ((Tnex+T)/2-Tenv)/(deltax/(lamda*A)+1/(ha*A));
}

//exo power
double Compute::getPexo(double T,double I,double SOC,int layer) {
    double R0 = getR(T,I,SOC,layer);
    return I*I*R0;
}

//PTC power
double Compute::getPptc(double T,double Tnex,double Pcool,double Pexo,int layer) {
    double Pptc = (Tnex-T)*Ctotal/dt[layer-1]+Pcool-Pexo;
        if (Pptc < 0){
            return 0;
        }
        else if (Pptc > 500){
            return 500;
        }
        return Pptc;
}

//delta temperature
double Compute::getDeltaT(double Pptc,double Pcool,double Pexo,int layer) {
    return (Pptc-Pcool+Pexo)*dt[layer-1]/Ctotal;
}

//delta SOC
double Compute:: getDeltaSoc(double parentT, double childT, double parentSOC,int parentLayer,double I){
    double Pcool = getPcool(parentT,childT);
    double Pexo = getPexo(parentT,I,parentSOC,parentLayer);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,parentLayer);
    double Qt = getQt(I,childT);
    double dsoc = (I+Pptc/Uptc)*(dt[parentLayer-1]/3600)/Qt;
    return  dsoc;
}



/******************* public *************************/

double Compute:: get_max_T(double parentT,int parentLayer,double parentSOC,double I)
{
    double Pptc = Pmax;
    double Pcool = getPcool(parentT,parentT);
    double Pexo = getPexo(parentT,I,parentSOC,parentLayer);
    return getDeltaT(Pptc,Pcool,Pexo,parentLayer) + parentT;

}

double Compute::get_min_T(double parentT,int parentLayer,double parentSOC,double I)
{
    double Pptc = 0;
    double Pcool = getPcool(parentT,parentT);
    double Pexo = getPexo(parentT,I,parentSOC,parentLayer);
    return getDeltaT(Pptc,Pcool,Pexo,parentLayer) + parentT;
}
/**
* define first layer temperature
* i :index of childNodes
* N :number of first layer nodes
* parentT :the temperature of parent
*
*/
double Compute:: get_firstLayer_T(int i,int N,double parentT,double SOC,double I){
    const int layer = 1;
    double Tmin = get_min_T(parentT,layer,I,SOC);
    double Tmax = get_max_T(parentT,layer,I,SOC);
    double const M = 2*N;
    double childT = ( M - 2 * i + 1) * Tmin/ M  + (2 * i - 1) * Tmax/ M ;

    printf("parentT:%lf\n",parentT); write << "parentT:"<<parentT<<endl;
    printf("childTmin:%lf\n",Tmin);write << "childTmin:"<<Tmin<<endl;
    printf("childTmax:%lf\n",Tmax);write << "childTmax:"<<Tmax<<endl;
    printf("childT:%lf\n\n",childT);write << "childT:"<<childT<<endl;

    return childT;

}
/**
* parentT :parent's temperature
* childT:child's temperature
* layer: parent's layer,begin with 1
*/
double Compute::cal_cost(double parentT,double childT,int parentLayer,double parentSOC,double I)
{
    double Qt = getQt(I,childT);
    double Pcool = getPcool(parentT,childT);
    double Pexo = getPexo(parentT,I, parentSOC,parentLayer);
    double Pptc = getPptc(parentT,childT,Pcool,Pexo,parentLayer);
    printf("Qt:%.4lf; Pcool:%.4lf; Pexo:%.4lf; Pptc:%.4lf; layer:%ld\n",Qt, Pcool, Pexo, Pptc,parentLayer);
    write << "Qt:"<<Qt<<"Pcool:"<<Pcool<<"Pptc:"<<Pptc<< "layer:"<<parentLayer<<endl;
    return (Pptc/Uptc+I) * (Q0/Qt) * (dt[parentLayer-1]/3600);

}


double Compute::getSoc(double parentT, double childT, double parentSOC,int parentLayer,double I){
    return parentSOC - getDeltaSoc( parentT,  childT,  parentSOC, parentLayer, I);
}
