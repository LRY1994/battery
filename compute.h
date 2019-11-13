#ifndef COMPUTE
#define COMPUTE

#include <vector>
#include "struct.h"
#include "./simulink/Lookuptable.h"
using namespace std;

class Compute
{
private:
     static constexpr  double Tenv = -20;
     static constexpr  double deltax = 0.01;
     static constexpr  double lamda = 0.03;
     static constexpr  double ha = 7.5;
     static constexpr  double A = 0.54;
     static constexpr  double Pmax = 500;
     static constexpr  double Q0 = 37;
     static constexpr  double Ctotal = 19501.272;
     static constexpr  double SOC0 = 1;
     static constexpr  double Uptc = 48;
    //  vector<Current_Area> current_data;
     LookuptableModelClass *rtObj;

 public:
    Compute(vector<Current_Area>);
 
     //Dt
     double  getDt(int layer);

     //I
     double  getI(int layer);
     

     //Qt
     double getQt(double I,double Tnex);

     //R0
     double getR(double T,double Pptc);

     //cooling power
     double getPcool(double T,double Tnex) ;

     //exo power
     double getPexo(double T,double I,double Pptc) ;

     //PTC power
     double getPptc(double T,double Tnex,double Pcool,double Pexo,int dt);

     //delta temperature
     double getDeltaT(double Pptc,double Pcool,double Pexo,int dt);

     //delta SOC
     double getDeltaSoc(double parentT, double childT, double parentSOC,int dt,double I);

     

     double getTime(double lastTime, int parentLayer);

     //return the max temperature of this node
     double get_max_T(double parentT, double parentSOC, int parentLayer);

     //return the min temperature of this node
     double get_min_T(double parentT, double parentSOC, int parentLayer);

     //return temperature of the first layer
     double get_firstLayer_T(int i, int N, double parentT, double parentSOC);

     //return the remmant soc of this node
     double getSoc(double parentT, double childT, double parentSOC, int parentLayer);

     //return the cost of this transfer from it's parentNode
     double cal_cost(double parentT, double childT, double parentSOC, int parentLayer);

     //return the cost of manual operation
     double getCost(double Pptc, double Qt, double I, int dt);

};

#endif // COMPUTE
