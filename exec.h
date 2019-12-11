#ifndef EXEC_H
#define EXEC_H

#include <vector>
#include "struct.h"
using namespace std;

class Exec
{

private:   
    double all_min_cost ;
    vector<double> all_min_path;
    double rootT;     
    void buildOneTree(double T, int height, int current_index);
    void buildMultiTree(vector<int> depth);
    void makePoints();
    void build(char *file,  int fromPos, int toPos);

  public:
    vector<double> pointX;
    vector<double> pointY;
    Exec(char *file, int fromPos, int toPos,double a_rootT); 
    double getCost();
    double getX(int index);
    double getY(int index);
    int getPointSize();
};

#endif // EXEC_H
