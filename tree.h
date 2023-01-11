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


class Attributes{
    int ParTree;
    int ParNode;
    int RankTree;
    int RankNode;
public:
    Attributes(){ParTree=-1;ParNode=-1;RankTree=-1;RankNode=-1;}
    void setPar(int x,bool type){type?ParNode=x:ParTree=x;}
    int getPar(bool type){if(type)return ParNode;return ParTree;}
    void setRank(int x,bool type){type?RankNode=x:RankTree=x;}
    int getRank(bool type){if(type)return RankNode;return RankTree;}
};

class Node{
    int level;
    int area;
    int highest;
    int vol;
    vector<Node*> children;
    vector<int> list_pixel;
public:
    Node(){level=-1; children={}; area=-1, highest=-1;vol=-1;list_pixel={};}
    Node(int l){level=l; children={}; area=1, highest=l;vol=1;list_pixel={};}
    ~Node(){int n=children.size();for (int i =0; i<n; ++i)delete children[i];}
    void addChildren(vector<Node*> new_children){children.insert(children.end(), new_children.begin(), new_children.end());}
    void addChild(Node* new_child){children.push_back(new_child);}
    vector<Node*> getChildren(){return children;}
    void setArea(int a){area=a;}
    int getArea(){return area;}
    void setHighest(int h){highest=h;}
    int getHighest(){return highest;}
    int getLevel(){return level;}
    int getVol(){return vol;}
    void setVol(int v){vol=v;}
    void addPixel(int p){list_pixel.push_back(p);}
    vector<int> getPixel(){return list_pixel;}
};

void MakeSet(const int x,Attributes* att,const bool type);// we define true for node and false for tree
int Find(const int x,Attributes* att,const bool type);
int Link(int x, int y, Attributes* att,const bool type);

int MergeNode(int n1, int n2, Attributes* att,Node* Nodes);

void sort(int* V, const byte* F, const int N);

vector<int> valid_neighbors(const vector<bool> &Processed,const int p, const byte* F, const int width,const int height);


void BuildComponentTree(int* V,const int width,const int height,const byte* F, Attributes* att, Node* Nodes,  int &root, int* M, int* lowest_node);

int computeVolume(Node* n);

void drawTree(byte* image,Node* root);

void display(Node* root,string prefix, string indent);

void Pixel_under_n(vector<int> &vector_pixel,Node* n);
