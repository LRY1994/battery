#include "tree.h"
#include "struct.h"
#include <stack>
#include "var.h"
#include<fstream>
using namespace std;


Tree::Tree(BTNode *&root,
            double a_rootT,
            int a_firstLayerNum,
            int a_layer,
            double a_I):root(root){
    rootT = a_rootT;
    layer = a_layer;
    firstLayerNum = a_firstLayerNum;
    I = a_I;
}

void Tree::create(){

        vector<BTNode *> list;
        int layer_count = 0;

        //root
        root = new BTNode();
        root->temperature = rootT;
        root->all_cost = 0;
        root->layer = 1;
        root->SOC = 1;
        root->path.push_back(root->temperature);
        layer_count ++;

        //first layer，下标从1开始
        BTNode * tmp;
        int i=0;
        while(i < firstLayerNum){
            i++;
            tmp = new BTNode();
            tmp->temperature = ComputeObj->get_firstLayer_T(i,firstLayerNum,root->temperature,root->SOC,I);
            tmp->SOC =  ComputeObj->getSoc(root->temperature,tmp->temperature,root->SOC,root->layer, I);
            tmp->all_cost = 0;
            tmp->layer = root->layer + 1 ;
            tmp->path.push_back(root->temperature);

            root->children.push_back(tmp);
            list.push_back(tmp);
        }
        layer_count ++;

        vector<BTNode *> tmplist;
        while (layer_count  < layer)
        {

            for (int i = 0; i < list.size(); i++)
            {
                BTNode *node = list[i];
                double high = ComputeObj->get_max_T(node->temperature,node->layer,node->SOC,I) ;
                double low =  ComputeObj->get_min_T(node->temperature,node->layer,node->SOC,I) ;
                double dist = high - low;
                //left node
                tmp = new BTNode();
                tmp->temperature = low + dist * 0.25;
                tmp->all_cost = 0;
                tmp->layer = node->layer + 1;
                tmp->SOC =  ComputeObj->getSoc(node->temperature,tmp->temperature,node->SOC,node->layer, I);
                node->children.push_back(tmp);
                tmplist.push_back(tmp);

                //right node
                tmp = new BTNode();
                tmp->temperature = high - dist * 0.25;
                tmp->all_cost = 0;
                tmp->layer = node->layer + 1;
                tmp->SOC =  ComputeObj->getSoc(node->temperature,tmp->temperature,node->SOC,node->layer, I);
                node->children.push_back(tmp);
                tmplist.push_back(tmp);
            }

            list = tmplist;
            layer_count ++;
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
                double power = ComputeObj ->cal_cost(parentT, childT, node->layer ,node->SOC, I);
                child->all_cost = node->all_cost + power;

                // printf("parentT:%lf;childT:%lf;layer:%ld;power:%lf\n",parentT,childT,node->layer, power);

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

