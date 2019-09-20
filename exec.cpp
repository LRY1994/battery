#include <cstdio>
#include <fstream>
#include "var.h"
#include "tree.h"
#include "spline.h"
#include "exec.h"
#include "current.h"
using namespace  std;

Compute *ComputeObj=nullptr;
double min_cost = 99999999;
vector<double> min_path;
ofstream write;  


Exec::Exec(double a_rootT,int a_first_layer_num,int a_other_layer_num)
{  
    rootT = a_rootT;
    first_layer_num = a_first_layer_num;
    other_layer_num = a_other_layer_num;

    //handle current data
    Current *CurrentObj = new Current(); 
    char *p=(char*)"current_data\\UDDS.txt";
    vector<Point> data = CurrentObj->readData(p,0 ,30);
    vector<Current_Area>  current_data = CurrentObj->processData(data);

    layer = current_data.size();

    //initilize Compute
    ComputeObj= new  Compute(current_data);

    write.open("log.txt",ios::out);  //将OF与“study.txt”文件关联起来
}


/**
 * @description 执行函数
 * @param {int} segment ，时间间隔数
 * @param {double} initialVal 初始温度
 * @param {int} firstLayerNum 第一层节点数
 * @returns {vector<double>} 返回m数组
 */
vector<double> Exec::getMArray()
{

    //build the tree;
    BTree tree;
    Tree batteryTree(tree, rootT, first_layer_num, other_layer_num, layer);
    batteryTree.create();

    write<<"build the tree done.... "<<endl;

    

    //serach the tree;
    batteryTree.depthFirstSearch();
    write<<"search the tree done.... "<<endl;


    write<<"the min_cost of Ah is "<<min_cost<<endl;
    printf("the min_cost of Ah is %.2lf\n",min_cost);
    printf("the temperature of min_cost path is:");
    write<<"the temperature of min_cost path is:";

    double t;
    for (int i = 0; i < min_path.size(); i++)
    {
        if(i > 0) t = ComputeObj->getTime(pointX[i-1],i);
        else t = 0;
        point.push_back(Point(t,min_path[i]));
        pointX.push_back(t);
        pointY.push_back(min_path[i]);
        printf("%.2lf,", min_path[i]);
        write<<min_path[i]<<",";
    }

    //begin to spline
    Spline *spline=new Spline();
    printf("\nstart splining......\n"); 
    write<<"\nstart splining......\n"<<endl;

    mArray = spline->interpolation(point,2);

    return mArray;

}

double Exec:: getM(int index){
    return mArray[index];
}

double Exec::getX(int index){
 return pointX[index];
}

double Exec::getY(int index){
 return pointY[index];
}
