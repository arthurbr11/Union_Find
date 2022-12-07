#include "tree.h"

void MakeSet(int x,attributes* att,const bool type){
    type? att[x].setParNode(x),att[x].setRankNode(0) : att[x].setParTree(x), att[x].setRankTree(0);
}



int Find(int x,attributes* att,const bool type){
    if (type){
        if(att[x].getParNode()!=x)
            att[x].setParNode(Find(att[x].getParNode(),att,type));
        return att[x].getParNode();
    }
    else{
        if(att[x].getParTree()!=x)
            att[x].setParTree(Find(att[x].getParTree(),att,type));
        return att[x].getParTree();
    }

}

int Link(int x, int y, attributes* att,const bool type){
    if (type){
        if(att[x].getRankNode()>att[y].getRankNode()){
            int z = x;
            x=y;
            y=z;
        }
        if(att[x].getRankNode()==att[y].getRankNode())
            att[y].setRankNode(att[y].getRankNode()+1);
        att[x].setParNode(y);
        return y;
    }
    else{
        if(att[x].getRankTree()>att[y].getRankTree()){
            int z = x;
            x=y;
            y=z;
        }
        if(att[x].getRankTree()==att[y].getRankTree())
            att[y].setRankTree(att[y].getRankTree()+1);
        att[x].setParTree(y);
        return y;
    }
}

int MergeNode(int n1, int n2, attributes* att, Node* Nodes){
    int tmpNode= Link(n1, n2,att,node);
    int tmpNode2=-1;
    if(tmpNode==n2){
        Nodes[n2].addChildren(Nodes[n1].getChildren());
        tmpNode2=n1;
    }
    else{
        Nodes[n1].addChildren(Nodes[n2].getChildren());
        tmpNode2=n2;
    }
    Nodes[tmpNode].setArea(Nodes[tmpNode].getArea()+Nodes[tmpNode2].getArea());
    Nodes[tmpNode].setHighest(Nodes[tmpNode].getHighest()+Nodes[tmpNode2].getHighest());
    return tmpNode;
}

void sort(int* V, const byte* F, const int N){ // decreasing sort
    vector<int> Levels[256];
    for(int i=0; i<N; i++){
        Levels[F[i]].push_back(i);
    }
    int current_pos=0;
    for(int i=0;i<256;i++){
        for(int j=0;j<Levels[255-i].size();j++){
            V[current_pos]=Levels[255-i][j];
            current_pos++;
        }
    }
}


vector<int> valid_neighbors(const vector<bool> &Processed,const int p, const byte* F, const int width,const int height){
    vector<int> E={};
    if(p%width!=0 and not Processed[p-1] and F[p-1]>=F[p])
        E.push_back(p-1);
    if(p%(width-1)!=0 and not Processed[p+1] and F[p+1]>=F[p])
        E.push_back(p+1);
    if(p>=width and not Processed[p-width] and F[p-width]>=F[p])
        E.push_back(p-width);
    if(p<(height-1)*width and not Processed[p+width] and F[p+width]>=F[p])
        E.push_back(p+width);
    return E;
}





void BuildComponentTree(int* V,const int width,const int height,const byte* F,attributes* att, Node* Nodes,  int &Root, int* M, int* lowest_node){
    const int N=width*height;
    sort(V,F,N);
    for(int i=0; i<N;i++){
        MakeSet(V[i],att,node);
        MakeSet(V[i], att,tree);
        Nodes[V[i]]=Node(F[V[i]]);
        lowest_node[V[i]]=V[i];
    }
    vector<bool> Processed(N, false);
    for(int i=0; i<N;i++){
        int curTree=Find(V[i], att,tree);
        int curNode=Find(lowest_node[curTree], att,node);
        vector<int> valid_neighborhood = valid_neighbors(Processed, V[i], F, width, height);
        for(int j=0;j<valid_neighborhood.size();j++){
            int adjTree=Find(valid_neighborhood[j], att,tree);
            int adjNode=Find(lowest_node[adjTree], att,node);
            if(curNode!=adjNode){
                if(Nodes[curNode].getLevel()==Nodes[adjNode].getLevel())
                    curNode=MergeNode(adjNode,curNode, att, Nodes);
                else{
                    Nodes[curNode].addChild(&Nodes[adjNode]);
                    Nodes[curNode].setArea(Nodes[curNode].getArea()+Nodes[adjNode].getArea());
                    Nodes[curNode].setHighest(max(Nodes[curNode].getHighest(),Nodes[adjNode].getHighest()));
                    }
                curTree=Link(adjTree,curTree,att,tree);
                lowest_node[curTree]=curNode;
            }
        }
        Processed[V[i]]=true;
    }
    Root=lowest_node[Find(Find(0,att,node),att,tree)];
    for(int i=0; i<N;i++){
        M[i]=Find(V[i],att,node);
    }
}

int computeVolume(Node* n){
    int vol = n->getArea();
    vector<Node*> children = n->getChildren();
    if(children.size()>0){
        for(int i=0;i<children.size();i++)
            vol += computeVolume(children[i])+children[i]->getArea()*(children[i]->getLevel()-n->getLevel());
    }
    n->setVol(vol);
    return vol;

}
