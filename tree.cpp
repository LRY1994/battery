#include "tree.h"
#include "struct.h"
#include <stack>
#include "var.h"
#include <fstream>
#include <iostream>
using namespace std;

Tree::Tree(BTNode *&root,
           double a_rootT,
           int a_layer,
           int a_root_current_index) : root(root)
{
    rootT = a_rootT;
    height = a_layer;
    min_cost = 9999999;
    min_path.clear();
    root_current_index = a_root_current_index;
}

void Tree::create(){
    
        vector<BTNode *> list;
        int height_count = 0;
        
        //root
        root = new BTNode(
            rootT,
            root_current_index,
            1
        );
        // root = new BTNode();
        // root->temperature = rootT;
        // root->all_cost = 0;
        // root->current_index = root_current_index;
        // root->SOC = 1;
        
        root->path.push_back(rootT);
        height_count++;
        list.push_back(root);
       

        BTNode * tmp;
        vector<BTNode *> tmplist;
        while (height_count + 1 <= height)
        {
           

            for (int i = 0; i < list.size(); i++)
            {
                
                BTNode *node = list[i];
                double high = g_ComputeObj->get_max_T(node->temperature, node->SOC, node->current_index);
                double low =  g_ComputeObj->get_min_T(node->temperature, node->SOC, node->current_index);
                double dist = high - low;
                int degree = g_ComputeObj->getDegree(node->current_index);
               

                double seg = dist / degree;

                int j = 0;
                //create child nodes
                while (j < degree)
                {
                    tmp = new BTNode();
                    tmp->all_cost = 0;
                    tmp->current_index = node->current_index + 1;
                    tmp->temperature = low + seg * j; //j从0开始计算，温度从小到大排序
                    tmp->SOC = g_ComputeObj->getSoc(node->temperature, tmp->temperature, node->SOC, node->current_index);
                   
                    // tmp = new BTNode(
                    //     low + seg * j,           //temperaturej从0开始计算，温度从小到大排序
                    //     node->current_index + 1, //current_index
                    //     g_ComputeObj->getSoc(node->temperature, tmp->temperature, node->SOC, node->current_index)
                    // );
                   
                    node->children.push_back(tmp);
                    tmplist.push_back(tmp);
                    j++;
                }
            }
            list = tmplist;
            height_count++;
            tmplist.clear();
        }
}

void Tree::depthFirstSearch(){

        stack<BTNode *> nodeStack;
        nodeStack.push(root);
        BTNode *node;

        vector<double> tmp;
        while (!nodeStack.empty())
        {

            node = nodeStack.top();
            // printf("%lf\n", node->temperature);
            nodeStack.pop();

            BTNode *child;
            for(int i = 0;i < node->children.size();i++){
                child = node->children[i];

                double parentT = node->temperature;
                double childT = child->temperature;
                double power = g_ComputeObj->cal_cost(parentT, childT, node->SOC, node->current_index);
                child->all_cost = node->all_cost + power;
                
                if (child->all_cost < min_cost)
                { //pruning
                    child->path = node->path;
                    child->path.push_back(child->temperature);
                    nodeStack.push(child);
                }
            }
            if (node->children.size() == 0)
            { //child node
                if (node->all_cost < min_cost)
                {
                    min_cost = node->all_cost;
                    min_path = node->path;
                }
            }
        }
    }

 double  Tree:: getMinCost(){
     return min_cost;
 }
 vector<double> Tree :: getPath(){
     return min_path;
 }
