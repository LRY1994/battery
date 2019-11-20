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
            double a_rootT,
            int a_first_layer_num,
            int a_other_layer_num )
{  
    rootT = a_rootT;
    first_layer_num = a_first_layer_num;
    other_layer_num = a_other_layer_num;
    all_min_cost = 0;
    write.open("log.txt", ios::out); //将OF与“log.txt”文件关联起来

    build(file, fromPos, toPos);
}

void Exec::build(char *file,  int fromPos,  int toPos){
    //handle current data
    Current *CurrentObj = new Current();
    vector<Point> data = CurrentObj->readData(file, fromPos, toPos);
    vector<Current_Area> current_data  = CurrentObj->processData(data);

    //initilize Compute
    g_ComputeObj = new Compute(current_data);
    g_CurrentData = current_data;

    buildMultiTree();
    makePoints(); 
}

//build the tree; 
void Exec::buildMultiTree()
{   
    double next_T = rootT;
    int size = g_CurrentData.size();
    printf("It amouts to %d current areas\n", size);
    vector<int> depth = getDepth();
    int acce = 0;

    int j = 0;
    int root_current_index;
    for (int i = 0; i < depth.size(); i++)
    {
        printf("\nthe %d tree's height is %d:\n", j, depth[i]);
        buildOneTree(next_T, depth[i], acce);
        acce += (depth[i]-1);
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
            t = g_ComputeObj->getTime(pointX[i - 1], i - 1);
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
    Tree batteryTree(tree, T, first_layer_num, other_layer_num, height, current_index);
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

//树深度与节点度自动优化算法
vector<int> Exec::getDepth()
{
    vector<int> result;
    vector<int> degree;
    //节点度的计算
    // cout << g_CurrentData.size();
    for (int i = 0; i < g_CurrentData.size(); i++)
    {
        if(g_CurrentData[i].dt>300)
            degree.push_back(4);
        else
            degree.push_back(3);
    }
    // printf("degree is", degree[2]);
    //树深度的计算
    int from = 0;
    int j = 0; //j为degree中第几个元素
    const int limitation = 25000000; //时间复杂度的限制,复杂度即节点个数
    int complexity, pre_com, prepre_com;
    int n;
    while (j < degree.size() + 1)
    {
        n = j - from + 1; //n为样本个数，也就是深度
        // cout << "n:" << n << "j:" << j << "f:" << from;
        if (j == from)
        { //初始化
             complexity = 1;
        }
        else if (j-1 == from)
        {
            pre_com = complexity;
            complexity = pre_com + degree[j];
        }
        else
        {
            if (n > 15)
            { //如果超过最大深度，则放弃该点
                result.push_back(n-1);
                j--;
                from = j;
                j--;
            }
            else
            {
                prepre_com = pre_com;
                pre_com = complexity;
                complexity = pre_com + (pre_com - prepre_com) * degree[j];
                if (complexity>limitation)
                { //如果超过复杂度范围，则放弃该层
                    result.push_back(n-1);
                    j--;
                    from = j;
                    j--;
                }
            }
        }
        // cout <<"de:"<<degree[j]<<endl;
        j++;
        // cout << j;
    }
    result.push_back(n);
    
    for (int k = 0; k < result.size();k++){
        cout << "the depth of " << k << "-th tree is " << result[k] << endl;
    }
    return result;
}
