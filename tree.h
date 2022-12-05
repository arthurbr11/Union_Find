#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <Imagine/Graphics.h>
#include <cmath>
using namespace Imagine;
using namespace std;


class Node{
    int level;
    int area;
    int highest;
    vector<Node> children;
public:
    Node(){level=-1; children={}; area=-1, highest=-1;}
    Node(int l){level=l; children={}; area=1, highest=l;}
    ~Node(){} //A faire
    void addChildren(vector<Node> new_children){children.insert( children.end(), new_children.begin(), new_children.end());}
    void addChild(Node new_child){children.push_back(new_child);}
    vector<Node> getChildren(){return children;}
    void setArea(int a){area=a;}
    int getArea(){return area;}
    void setHighest(int h){highest=h;}
    int getHighest(){return highest;}
    int getLevel(){return level;}
};

void MakeSet(int x, int* Par, int* Rank);
int Find(int x, int* Par);
int Link(int x, int y, int* Par, int* Rank);

int MergeNode(int n1, int n2);

void sort(int* V, byte* F, int N);

vector<int> valid_neighbors(vector<bool> Processed, int p, byte* F, int width, int height);


void BuildComponentTree(int*V, int width, int height, byte* F, int*ParNode, int*RankNode, int*ParTree, int*RankTree, Node* Nodes,  int &Root, int*M, int*lowest_node);

