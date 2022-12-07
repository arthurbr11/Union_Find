#pragma once
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <Imagine/Graphics.h>
#include <cmath>
using namespace Imagine;
using namespace std;

const bool node = true;
const bool tree = false;


class attributes{
    int ParTree;
    int ParNode;
    int RankTree;
    int RankNode;
public:
    attributes(){ParTree=-1;ParNode=-1;RankTree=-1;RankNode=-1;}
    void setParTree(int x){ParTree=x;}
    int getParTree(){return ParTree;}
    void setParNode(int x){ParNode=x;}
    int getParNode(){return ParNode;}
    void setRankTree(int x){RankTree=x;}
    int getRankTree(){return RankTree;}
    void setRankNode(int x){RankNode=x;}
    int getRankNode(){return RankNode;}
};

class Node{
    int level;
    int area;
    int highest;
    int vol;
    vector<Node*> children;
public:
    Node(){level=-1; children={}; area=-1, highest=-1;vol=-1;}
    Node(int l){level=l; children={}; area=1, highest=l;vol=1;}
    ~Node(){int n=children.size();for (int i =0; i<n; ++i)delete children[i];}
    void addChildren(vector<Node*> new_children){children.insert( children.end(), new_children.begin(), new_children.end());}
    void addChild(Node* new_child){children.push_back(new_child);}
    vector<Node*> getChildren(){return children;}
    void setArea(int a){area=a;}
    int getArea(){return area;}
    void setHighest(int h){highest=h;}
    int getHighest(){return highest;}
    int getLevel(){return level;}
    int getVol(){return vol;}
    void setVol(int v){vol=v;}
};

void MakeSet(int x,attributes* att,const bool type);// we define true for node and false for tree
int Find(int x,attributes* att,const bool type);
int Link(int x, int y, attributes* att,const bool type);

int MergeNode(int n1, int n2, attributes* att,Node* Nodes);

void sort(int* V, const byte* F, const int N);

vector<int> valid_neighbors(const vector<bool> &Processed,const int p, const byte* F, const int width,const int height);


void BuildComponentTree(int* V,const int width,const int height,const byte* F, attributes* att, Node* Nodes,  int &Root, int* M, int* lowest_node);

int computeVolume(Node* n);
