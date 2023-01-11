#include "tree.h"

void MakeSet(const int x,Attributes* att,const bool type){
    att[x].setPar(x,type),att[x].setRank(0,type);
}

int Find(const int x,Attributes* att,const bool type){
    if(att[x].getPar(type)!=x)
        att[x].setPar(Find(att[x].getPar(type),att,type),type);
    return att[x].getPar(type);

}

int Link(int x, int y, Attributes* att,const bool type){
    if(att[x].getRank(type)>att[y].getRank(type)){
        int z = x;
        x=y;
        y=z;
    }
    if(att[x].getRank(type)==att[y].getRank(type))
        att[y].setRank(att[y].getRank(type)+1,type);
    att[x].setPar(y,type);
    return y;

}

int MergeNode(int n1, int n2, Attributes* att, Node* Nodes){
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
    if(p%width!=0 and Processed[p-1] and F[p-1]>=F[p])
        E.push_back(p-1);
    if(p%width!=width-1 and Processed[p+1] and F[p+1]>=F[p])
        E.push_back(p+1);
    if(p>=width and Processed[p-width] and F[p-width]>=F[p])
        E.push_back(p-width);
    if(p<(height-1)*width and Processed[p+width] and F[p+width]>=F[p])
        E.push_back(p+width);
    return E;
}





void BuildComponentTree(int* V,const int width,const int height,const byte* F,Attributes* att, Node* Nodes,  int &root, int* M, int* lowest_node){
    const int N=width*height;
    sort(V,F,N);
    for(int i=0; i<N;i++){
        MakeSet(V[i],att,node);
        MakeSet(V[i], att,tree);
        Nodes[V[i]]=Node(F[V[i]]);
        lowest_node[V[i]]=V[i];
    }
    vector<bool> Processed(N,false);
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
    root=lowest_node[Find(Find(0,att,node),att,tree)];
    for(int i=0; i<N;i++){
        M[V[i]]=Find(V[i],att,node);
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

void drawTree(byte* image,Node* root){
    for (int i=0;i<root->getChildren().size();i++)
        drawTree(image,root->getChildren()[i]);
    for (int i=0;i<root->getPixel().size();i++)
        image[root->getPixel()[i]]=root->getLevel();
}

void display(Node* root,string prefix, string indent){
    cout << prefix<<root->getLevel()<< endl;
    for (int i=0;i<root->getChildren().size();i++){
        display(root->getChildren()[i],prefix+indent,indent);
    }
};
void Pixel_to_draw(vector<int> &vector_pixel,Node* n){
    for (int i=0;i<n->getPixel().size();i++){
        vector_pixel.push_back(n->getPixel()[i]);
    }
    if (n->getChildren().size()==0)
        return ;
    for (int i=0;i<n->getChildren().size();i++){
        Pixel_to_draw(vector_pixel,n->getChildren()[i]);
    }
    return ;


}
