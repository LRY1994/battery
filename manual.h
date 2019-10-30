#ifndef MANUAL_H
#define MANUAL_H

#include "struct.h"
#include <vector>
using namespace std;
class Manual
{

public:

    double fromT;
    double toT;

    Manual(double a_fromT, int a_toT);
    double getCost();
};

#endif // MANUAL_H
