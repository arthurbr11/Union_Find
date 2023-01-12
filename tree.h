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
const int AREA=0;
const int HIGHEST=1;
const int VOLUME=2;



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
    int mark;
    int nb_children;
    Node* parent;
    vector<Node*> children;
    vector<int> list_pixel;
public:
    Node(){level=-1; children={}; area=-1, highest=-1;vol=-1;mark=0;nb_children=0;list_pixel={};}
    Node(int l){level=l; children={}; area=1, highest=l;vol=1;mark=0;nb_children=0;list_pixel={};}
    bool operator ==(Node n){if (level==n.level && area==n.area && highest==n.highest && vol==n.vol && mark==n.mark && nb_children==n.nb_children && list_pixel==n.list_pixel)return true;return false;}

    void setParent(Node* new_parent){parent=new_parent;}
    Node* getParent(){return parent;}
    ~Node(){int n=children.size();for (int i =0; i<n; ++i)delete children[i];}
    void addChildren(vector<Node*> new_children){children.insert(children.end(), new_children.begin(), new_children.end());nb_children+=new_children.size();}
    void addChild(Node* new_child){children.push_back(new_child);nb_children++;}
    vector<Node*> getChildren(){return children;}
    void setArea(int a){area=a;}
    int getArea(){return area;}
    void setHighest(int h){highest=h;}
    int getHighest(){return highest;}
    void setLevel(int l){level=l;}
    int getLevel(){return level;}
    int getVol(){return vol;}
    void setVol(int v){vol=v;}
    int getMark(){return mark;}
    void setMark(int m){mark=m;}
    int getNbChildren(){return nb_children;}
    void setNbChildren(int nb){nb_children=nb;}
    void addPixel(int p){list_pixel.push_back(p);}
    vector<int> getPixel(){return list_pixel;}
};

void MakeSet(const int x,Attributes* att,const bool type);// we define true for node and false for tree
int Find(const int x,Attributes* att,const bool type);
int Link(int x, int y, Attributes* att,const bool type);

int MergeNode(int n1, int n2, Attributes* att,Node* Nodes);

void sort(int* V, const byte* F, const int N);

vector<int> valid_neighbors(const vector<bool> &Processed,const int p, const byte* F, const int width,const int height);


void BuildComponentTree(int* V,const int width,const int height,const byte* F, Attributes* att, Node* Nodes,Node* nodeRoot, int &root, int* M, int* lowest_node);

int computeVolume(Node* n);

void drawTree(byte* image,Node* nodeRoot);
void make_parent(Node* nodeRoot,Node* parent);

void display(Node* nodeRoot,string prefix, string indent);


void Pixel_under_n(vector<int> &vector_pixel,Node* n);
void draw(Node* n,int width, int height);
void draw_with_parent(Node* n,int width, int height);



void inverseTree(Node* nodeRoot);
void sortVectorPixelRef(const int width,const int height ,const int caracteristic,const int* M,Node* Nodes,vector<int>& ListPixelReference);
int numberLeaf(Node* Nodes);
int firstLeaf(vector<int>& ListPixelWhichStay,Node* Nodes);
int toPixelRef(Node n, Node* Nodes,vector<int> ListPixelReference);

byte* Keep_N_Lobes (int* V,const int width,const int height,const int* M,Node* Nodes,Node* nodeRoot,int root,const int caracteristic, const int N);
int RemoveLobe(int c,Node* Nodes,vector<int>ListPixelReference);


bool is_under(Node* n1,Node* n2); // n2 is under n1

Node* get_parent_commun(Node* n1,Node* n2,Node* nodeRoot);
