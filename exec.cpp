#include <cstdio>
#include<iostream>
#include <fstream>
#include "var.h"
#include "tree.h"
#include "spline.h"
#include "exec.h"
#include "current.h"
using namespace std;

Compute *ComputeObj=nullptr;
vector<Current_Area> current_data;
ofstream write;

Exec::Exec(double a_rootT,int a_first_layer_num,int a_other_layer_num)
{  
    rootT = a_rootT;
    first_layer_num = a_first_layer_num;
    other_layer_num = a_other_layer_num;
    all_min_cost = 0;

    write.open("log.txt", ios::out); //将OF与“log.txt”文件关联起来
    //handle current data
    Current *CurrentObj = new Current();
    char *file=(char*)"current_data\\24hours.txt";
    vector<Point> data = CurrentObj->readData(file, 0, 40000);
    current_data = CurrentObj->processData(data);
   

   //initilize Compute
   ComputeObj = new Compute(current_data);

}


/**
 * @description 执行函数
 * @param {int} segment ，时间间隔数
 * @param {double} initialVal 初始温度
 * @param {int} firstLayerNum 第一层节点数
 * @returns {vector<double>} 返回m数组
 */
void Exec::buildMultiTree()
{

    //build the tree;
    

    int height = 10;//最大高度10
    int next_T = rootT;

    int size = current_data.size();
    printf("It amouts to %d current areas\n", size);
    int j = 0;
    for ( int i = 0; i <size; i += height)
    {
        
        height = height < size - i ? height : size - i;
        printf("\nthe %d tree's height is %d:\n", j ,height);
        int root_current_index = i ==  0 ? 0 : i - 1;
        buildOneTree(next_T, height, root_current_index);
        next_T = all_min_path.back();
        j++;
    }
   

}

void Exec::makePoints()
{
    double t;
    for (int i = 0; i < all_min_path.size(); i++)
    {
        if (i > 0)
            t = ComputeObj->getTime(pointX[i - 1], i - 1);
        else
            t = 0;
        point.push_back(Point(t, all_min_path[i]));
        pointX.push_back(t);
        pointY.push_back(all_min_path[i]);
    }
}
// double Exec:: getM(int index){
//     return mArray[index];
// }

double Exec::getX(int index){
 return pointX[index];
}

double Exec::getY(int index){
 return pointY[index];
}

void Exec::buildOneTree(double T, int height,int current_index)
{
    BTree tree;
    Tree batteryTree(tree, T, first_layer_num, other_layer_num, height, current_index);
    batteryTree.create();
    
    //serach the tree;
    batteryTree.depthFirstSearch();
    double min_cost = batteryTree.getMinCost();
    all_min_cost += min_cost;

    write << "the min_cost of Ah is \n" << min_cost << endl;
    printf("the min_cost of Ah is %.2lf\n", min_cost);

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
double Exec:: getAllCost()
{
    return all_min_cost;

};
