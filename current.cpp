#include "current.h"
#include "struct.h"
#include <fstream>
#include <cstring>
#include "stdio.h"
#include "var.h"
#include "assert.h"
#include <iostream>
#include <sstream>
using namespace  std;

//data process
vector<Point> Current::readData( const char* fileName, int from, int to){
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
        if(i>to) break;
        i++;
    }
    readstream.close();
    // for(int i=0;i<points.size();i++)cout<<points[i].x<<","<<points[i].y<<endl;
    return points;

}

vector<Current_Area> Current:: processData(vector<Point>data){
    
    vector<Current_Area> result;

    int size = data.size();

    double d= 0;//分母
    double sum ;
    double sum2 ;
    double E=0,Epre=0;//平均数

    int from =0;
    int i = 0;
    while(i < size){
        double x = data[i].y;
        if(i == from){//初始化
            sum = x;
            sum2 = x * x;
            E = x;
        }else{
            int n = i - from + 1;
            sum += x;
            sum2 += x * x;
            Epre = E;
            E = sum / n;

            d = ( sum2 -(n * E * E)) / n;

            if(d>10){//如果大于方差
                result.push_back(
                    Current_Area(
                        Epre,
                        from,
                        i-1
                    )              
                );
                from = i;
                i--;
            }
        }

        i++;
    }

    result.push_back(
        Current_Area(
            E,
            from,
            i-1
        )              
    );


     for(int i=0;i<result.size();i++)
    //  cout<<result[i].current<<","<<result[i].from<<","<<result[i].to<<endl;
     write<<result[i].current<<","<<result[i].from<<","<<result[i].to<<endl;
     

    return result;

}