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
  return all_min_path.size();
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

    write << "the min_cost of Ah is \n" << min_cost << endl;
    printf("the min_cost of Ah is %lf\n", min_cost);

    vector<double> min_path = batteryTree.getPath();
    if(all_min_path.size()==0){
        all_min_path = min_path;
    }else{
        all_min_path.insert(all_min_path.end(), min_path.begin()+1, min_path.end());
    }
    
    
    printf("the temperature of min_cost path is:\n");
    for (int j = 0; j < min_path.size(); j++){
        printf("%.2lf,", min_path[j]);
        write << min_path[j] << ",";
    }
    cout << endl;
}
double Exec:: getCost()
{
    return all_min_cost;
}


void Exec:: optimize(){
    for (int i = 0; i + 2 < all_min_path.size();i++) {
        three(i, i + 1, i + 2);
    }


}

void three(double i,double j,double k){
    double it = all_min_path(i);
    double jt = all_min_path(j);
    double kt = all_min_path(k);
    vector<Point> pointList;
    pointList.push_back(Point(i, it));
    pointList.push_back(Point(k, kt));
    pointList.push_back(Point(j, jt));
    spline(pointList);

    pointList.pop_back();
    pointList.push_back(Point(j, jt + 0.5));
    spline(pointList);

    pointList.pop_back();
    pointList.push_back(Point(j, jt - 0.5));
    spline(pointList);
}
void spline(vector<Point> pointList)
{
    Spline *s = new Spline();
    vector<double> mVector = s->interpolation(pointList, 2);
    int a[3], b[3], c[3], d[3];
    for (int i = 0; i < 2; i++)
    {
        a[i] = pointList[0].y;
        double h = pointList[i + 1].x - pointList[i].x;
        b[i] = (pointList[i + 1].y - pointList[i].y) / h - h * mVector[i] / 2 - h * (mVector[i + 1] - mVector[i]) / 6;
        c[i] = mVector[i] / 2;
        d[i] = (mVector[i + 1] - mVector[i]) / (6 * h);
        double L = 0;
        for (int j = 1; j < h; j++)
        {
            double hh = pointList[i].x + j;
            double childy = a[i] + b[i] * (hh - 1) + c[i] * pow(hh - 1, 2) + d[i] * pow(hh - 1, 3);
            double parenty = a[i] + b[i] * hh + c[i] * hh * hh + d[i] * hh * hh * hh;
            L += g_ComputeObj->cal_cost(parenty, childy,);
        }
    }
}