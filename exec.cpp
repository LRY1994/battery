#include <cstdio>
#include<iostream>
#include <fstream>
#include "var.h"
#include "tree.h"
#include "spline.h"
#include "exec.h"
#include "current.h"

using namespace std;


Exec::Exec(char *file, 
            int fromPos, 
            int toPos,
            double a_rootT )
{  
    rootT = a_rootT;
    all_min_cost = 0;
    write.open("log.txt", ios::out); //将OF与“log.txt”文件关联起来
    build(file, fromPos, toPos);
}

void Exec::build(char *file,  int fromPos,  int toPos){
    //handle current data
    Current *CurrentObj = new Current(file, fromPos, toPos);
    
    //initilize Compute
    vector<Current_Area> current_data =CurrentObj->getData();
    g_CurrentData = current_data;
    g_ComputeObj = new Compute(current_data);

    buildMultiTree(CurrentObj->getDepth());
    makePoints(); 
}

//build the tree; 
void Exec::buildMultiTree(vector<int>depth)
{ 
    double next_T = rootT;
    int acce = 0;

    int j = 0;
    int root_current_index;
   
    for (int i = 0; i < depth.size(); i++)
    {        
        printf("\nthe %d tree's height is %d:\n", j, depth[i]);
        buildOneTree(next_T, depth[i], acce);
        acce += (depth[i] - 1);
        next_T = all_min_path.back();
        j++;
    }
}

void Exec::makePoints()
{
    double t;
    int size = all_min_path.size();
    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            t = pointX[i-1] + g_ComputeObj->getDt( i - 1);
        else
            t = 0;
        pointX.push_back(t);
        pointY.push_back(all_min_path[i]);
    }
}
int Exec::getPointSize(){
    return pointY.size();
}
double Exec::getX(int index){
 return pointX[index];
}

double Exec::getY(int index){
 return pointY[index];
}

void Exec::buildOneTree(double T, int height,int current_index)
{
  
    BTree tree;
    Tree batteryTree(tree, T,  height, current_index);
   
    batteryTree.create();

    //serach the tree;
    batteryTree.depthFirstSearch();
    double min_cost = batteryTree.getMinCost();

    all_min_cost += min_cost;

    // write << "the min_cost of Money is \n" << min_cost << endl;
    printf("the min_cost of Money is %lf\n", min_cost);

    vector<double> min_path = batteryTree.getPath();
    if(all_min_path.size()==0){
        all_min_path = min_path;
    }else{
        all_min_path.insert(all_min_path.end(), min_path.begin()+1, min_path.end());
    }
    
    
    printf("the temperature of min_cost path is:\n");
    for (int j = 0; j < min_path.size(); j++){
        printf("%.2lf,", min_path[j]);
        // write << min_path[j] << ",";
    }
    cout << endl;
}
double Exec:: getCost()
{
    return all_min_cost;
}

