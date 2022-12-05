#include "tree.h"

void MakeSet(int x, int* Par, int* Rank){
    Par[x]=x;
    Rank[x]=0;
}



int Find(int x, int* Par){
    if(Par[x]!=x)
        Par[x]=Find(x,Par);
    return Par[x];
}





int Link(int x, int y, int* Par, int* Rank){
    if(Rank[x]>Rank[y]){
        int z = x;
        x=y;
        y=z;
    }
    if(Rank[x]==Rank[y])
        Rank[y]++;
    Par[x]=y;
    return y;
}

int MergeNode(int n1, int n2, int* ParNode, int* RankNode, Node* Nodes){
    int tmpNode= Link(n1, n2, ParNode, RankNode);
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

void sort(int* V, byte* F, int N){ //Tri par ordre décroissant
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


vector<int> valid_neighbors(vector<bool> Processed, int p, byte* F, int width, int height){
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





void BuildComponentTree(int* V, int width, int height, byte* F, int* ParNode, int* RankNode, int* ParTree, int* RankTree, Node* Nodes,  int &Root, int* M, int* lowest_node){
    const int N=width*height;
    sort(V,F,N);
    for(int i=0; i<N;i++){
        MakeSet(V[i], ParTree, RankTree);
        MakeSet(V[i], ParNode, RankNode);
        Nodes[V[i]]=Node(F[V[i]]);
        lowest_node[V[i]]=V[i];
    }
    vector<bool> Processed(N, false);
    for(int i=0; i<N;i++){
        int curTree=Find(V[i], ParTree);
        int curNode=Find(lowest_node[curTree], ParNode);
        vector<int> valid_neighborhood = valid_neighbors(Processed, V[i], F, width, height);
        for(int j=0;j<valid_neighborhood.size();j++){
            int adjTree=Find(valid_neighborhood[j], ParTree);
            int adjNode=Find(lowest_node[adjTree], ParNode);
            if(curNode!=adjNode){
                if(Nodes[curNode].getLevel()==Nodes[adjNode].getLevel())
                    curNode=MergeNode(adjNode,curNode, ParNode, RankNode, Nodes);
                else{
                    Nodes[curNode].addChild(Nodes[adjNode]);
                    Nodes[curNode].setArea(Nodes[curNode].getArea()+Nodes[adjNode].getArea());
                    Nodes[curNode].setHighest(max(Nodes[curNode].getHighest(),Nodes[adjNode].getHighest()));
                    }
                curTree=Link(adjTree,curTree,ParTree,RankTree);
                lowest_node[curTree]=curNode;
            }
        }
    }
    Root=lowest_node[Find(Find(0,ParNode),ParTree)];
    for(int i=0; i<N;i++){
        M[i]=Find(V[i],ParNode);
    }
}
