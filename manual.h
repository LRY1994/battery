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


    Manual( char *file, int fromPos,   int toPos ,double a_fromT, double a_toT ); 
    double getCost();
};

#endif // MANUAL_H
