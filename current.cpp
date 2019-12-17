#include "current.h"
#include "struct.h"
#include <fstream>
#include <cstring>
#include "stdio.h"
#include "var.h"
#include "assert.h"
#include <iostream>
#include <sstream>
#include <math.h>
using namespace  std;

Current::Current(const char *fileName, int from, int to)
{
    processData(readData(fileName, from, to));
    setDegree();
    setDepth();
}

//transform data from text to "points" vector
vector<Point> Current::readData(const char *fileName, int from, int to)
{
    vector<Point> points;
    ifstream readstream;
    readstream.open(fileName,ios::in);
    assert(readstream.is_open());
    
    int i = 0;

    while (!readstream.eof()){
        string inbuf;
        getline(readstream, inbuf, '\n');         

        if(i<=to && i >=from){
           string xstr,ystr;
           istringstream is(inbuf);
           is>>xstr>>ystr;//分割空格
                  
            int x = atoi(xstr.c_str());
            double y = atof(ystr.c_str() ); 
            points.push_back(Point(x, y));     
        }
        if(i>to) break;//only read to 'to'
        i++;
    }
    readstream.close();
    // for(int i=0;i<points.size();i++)cout<<points[i].x<<","<<points[i].y<<endl;
    return points;
}

//process data
void Current:: processData(vector<Point>point){
    
    vector<Current_Area> result;
    vector<Current_Area> Raw1;

    int size = point.size();
    for (int i = 1; i < size;i++){
        Raw1.push_back(Current_Area((point[i - 1].y + point[i].y) / 2, i - 1, i));
    }
    Raw = Raw1;
    double d = 0; //方差
    double S = 0;//标准差
    double sum ;//和
    double sum2 ;//平方和
    double E = 0, Epre = 0; //平均数
    double Emin;//置信区间下限
    double Emax;//置信区间上限
    // const double t95[31] = {12.7062,4.3027,3.1824,2.7764,2.5706,2.4469,2.3646,2.3060,2.2622,2.2281,2.2010,2.1788,2.1604,2.1448,2.1315,2.1199,2.1098,2.1009,2.0930,2.0860,2.0796,2.0739,2.0687,2.0639,2.0595,2.0555,2.0518,2.0484,2.0452,2.0423,2.0395}; //t分布95置信值

    const int LIMIT = 801;
   
    int from = 0;
    int i = 0;//i为原始数据第几秒
    // cout << point[30651].y << endl <<point[50].y << endl;
    while(i < size){
        double x = point[i].y;
        if(i == from){//初始化
            sum = x;
            sum2 = x * x;
            E = x;
        }else{
                int n = i - from + 1;//n为样本个数                  
                sum += x;
                sum2 += x * x;
                Epre = E;
                E = sum / n;
                d = (sum2 - sum * sum / n) / n;
                double S = sqrt(d);
                Emin = E - 3 * S * sqrt(n) / sqrt(n - 1);
                Emax = E + 3 * S * sqrt(n) / sqrt(n - 1);
                // Emin = E - S * t95[n - 2] / sqrt(n - 1);
                // Emax = E + S * t95[n - 2] / sqrt(n - 1);
                // cout << "Emin:" << Emin << "  "
                //      << "Emax:" << Emax << "  ";
                //方差未知，对均值进行95%置信区间估计（t分布）
                if (n > LIMIT)
                { //如果超过指定个数，则放弃该点
                    result.push_back(Current_Area(Epre, from, i - 1));
                    i--;
                    from = i;
                    i--;
                }

                else if (x < Emin || x > Emax)
                { //如果不在区间内，则放弃该点
                    result.push_back(Current_Area(Epre, from, i - 1));
                    i--;
                    from = i;
                    i--;
                }
            }        
        i++;
    }
    result.push_back(Current_Area(E, from, i - 1));
    printf("the curent_areas are:(current,fromTime,toTime):\n");
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i].current << "  ,  " << result[i].from << "  ,  " << result[i].to << endl;
        // write << result[i].current << "  ,  " << result[i].from << "  ,  " << result[i].to << endl;
     }

     data = result;

    //  return result;
}


//tree height

//树深度与节点度自动优化算法
void Current::setDegree()
{

    //节点度的计算
    // cout << g_CurrentData.size();
    for (int i = 0; i < data.size(); i++)
    {
        double val = data[i].dt * g_ComputeObj->Pmax / g_ComputeObj->Ctotal;
        if (val <= 1)
        {
            
            data[i].degree = 2;
        }
        else if (val > 1 && val <= 8)
        {
            
            data[i].degree = 3;
        }
        else if (val > 8)
        {
            data[i].degree = 4;
        }
        // cout << val << endl;
        // cout << data[i].degree << endl;
    }
}
void Current::setDepth()
{
   
    vector<int> result;
    //树深度的计算
    int from = 0;
    int j = 0;                       //j为degree中第几个元素
    const int limitation = 80000000; //时间复杂度的限制,复杂度即节点个数
    int complexity, pre_com, prepre_com;
    int n;
    while (j < data.size() + 1)
    {
        n = j - from + 1; //n为样本个数，也就是深度
        // cout << "n:" << n << "j:" << j << "f:" << from;
        if (j == from)
        { //初始化
            complexity = 1;
        }
        else if (j - 1 == from)
        {
            pre_com = complexity;
            complexity = pre_com + data[j].degree;
        }
        else
        {
            if (n > 14)
            { //如果超过最大深度，则放弃该点
                result.push_back(n - 1);
                j--;
                from = j;
                j--;
            }
            else
            {
                prepre_com = pre_com;
                pre_com = complexity;
                complexity = pre_com + (pre_com - prepre_com) * data[j].degree;
                if (complexity > limitation)
                { //如果超过复杂度范围，则放弃该层
                    // cout << "complx:" << complexity;
                    result.push_back(n - 1);
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

    for (int k = 0; k < result.size(); k++)
    {
        cout << "the depth of " << k << "-th tree is " << result[k] << endl;
    }
    depth = result;
}
vector<Current_Area> Current::getData()
{
    return data;
}
vector<int> Current::getDepth()
{
    return depth;
}
vector<Current_Area> Current::getpoint()
{
    return Raw;
}