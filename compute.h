#ifndef COMPUTE
#define COMPUTE

#include <vector>
#include "struct.h"
#include "./simulink/Lookuptable.h"
using namespace std;

class Compute
{
private:
    const double Tenv1[25] = {-12, -13, -15, -12, -12, -16, -17, -15, -12, -9, -10, -7, -6, -5, -5, -5, -7, -11, -12, -12, -11, -11, -12, -12, -12};
    static constexpr double deltax = 0.01;
    static constexpr double lamda = 0.03;
    static constexpr double ha = 7;
    static constexpr double A = 0.54;
    static constexpr double SOC0 = 1;
    static constexpr double eta = 0.9;
    static constexpr double Q0 = 37;
    static constexpr double Uptc = 48;
    static constexpr double Ppump = 36;
    static constexpr double alpha = 1243.33;
    static constexpr double Bcyc = 0.0039;
    static constexpr double Bcal = 56128;
    static constexpr double Eacyc = 16361;
    static constexpr double Eacal = 62862;
    static constexpr double Zcyc = 0.5644; //ok
    static constexpr double Zcal = 0.4973;
    static constexpr double money_perAh = 0.0864;   //ok
    static constexpr double money_perSOH = 15096; //ok
    static constexpr double R_gas = 8.314;
    LookuptableModelClass *rtObj;
    vector<Current_Area> currentData;

public:
    static constexpr double Pmax = 300;
    static constexpr double Ctotal = 19501.272;

    Compute(vector<Current_Area>);

    //Dt
    double getDt(int layer);
    //I
    double getI(int layer);
    double getDegree(int layer);

    double getTime(int parentLayer);

    //Qt
    double getQt(double I, double Tnex);

    //R0
    double getR(double T, double Pptc);

    //cooling power
    double getPcool(double T, double Tnex, int parentLayer);

    //exo power
    double getPexo(double T, double I, double Pptc);

    //PTC power
    double getPptc(double T, double Tnex, double Pcool, double Pexo, int dt);

    //delta temperature
    double getDeltaT(double Pptc, double Pcool, double Pexo, int dt);

    //delta SOC
    double getDeltaSoc(double parentT, double childT, double parentSOC, int dt, double I);

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

    //return the cost of this transfer from it's parentNode
    double cal_opti_cost(double parentT, double childT, double parentSOC, int parentLayer);

    //return the cost of manual operation
    double getCost(double Pptc, double Qt, double I, int dt);

    double get_opti_Soc(double parentT, double childT, double parentSOC, int parentLayer);

    double getTenv(int parentLayer);
    };

#endif // COMPUTE
