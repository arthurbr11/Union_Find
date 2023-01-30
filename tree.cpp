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
    Nodes[tmpNode].setHighest(max(Nodes[tmpNode].getHighest(),Nodes[tmpNode2].getHighest()));
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





void BuildComponentTree(int* V,const int width,const int height,const byte* F,Attributes* att, Node* Nodes,Node* nodeRoot,  int &root, int* M, int* lowest_node){
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
    for (int i=0;i<N;i++){
        Nodes[M[i]].addPixel(i);
    }
    *nodeRoot=Nodes[root];
    make_parent(nodeRoot,nodeRoot);
    computeVolume(nodeRoot);
}

int computeVolume(Node* n){
    int vol = n->getArea();
    if(n->getNbChildren()>0){
        for(int i=0;i<n->getNbChildren();i++)
            vol += computeVolume(n->getChildren()[i])+n->getChildren()[i]->getArea()*(n->getChildren()[i]->getLevel()-n->getLevel());
    }
    n->setVol(vol);
    return vol;

}

void drawTree(byte* image,Node* nodeRoot){
    for (int i=0;i<nodeRoot->getNbChildren();i++)
        drawTree(image,nodeRoot->getChildren()[i]);
    for (int i=0;i<nodeRoot->getPixel().size();i++)
        image[nodeRoot->getPixel()[i]]=nodeRoot->getLevel();
}
void make_parent(Node* nodeRoot,Node* parent){
    nodeRoot->setParent(parent);
    for (int i=0;i<nodeRoot->getNbChildren();i++)
        make_parent(nodeRoot->getChildren()[i],nodeRoot);
};


void display(Node* nodeRoot,string prefix, string indent){
    cout << prefix<<nodeRoot->getLevel()<< endl;
    for (int i=0;i<nodeRoot->getNbChildren();i++){
        display(nodeRoot->getChildren()[i],prefix+indent,indent);
    }
};
void Pixel_under_n(vector<int> &vector_pixel,Node* n){
    for (int i=0;i<n->getPixel().size();i++){
        vector_pixel.push_back(n->getPixel()[i]);
    }
    if (n->getNbChildren()==0)
        return ;
    for (int i=0;i<n->getNbChildren();i++){
        Pixel_under_n(vector_pixel,n->getChildren()[i]);
    }
    return ;
}
void draw(Node* n,int width, int height){
    vector<int> vetcor_pixel={};
    Pixel_under_n(vetcor_pixel,n);

    for (int i=0;i<vetcor_pixel.size();i++){
        int p=vetcor_pixel[i];
        drawPoint(p%width,p/width,RED);
    }
}
void draw_with_parent(Node* n,int width, int height){
    draw(n,width,height);
    for (int i=0;i<n->getPixel().size();i++){
        int p=n->getPixel()[i];
        drawPoint(p%width,p/width,GREEN);
    }
}

void inverseTree(Node* nodeRoot){
    nodeRoot->setLevel(255-nodeRoot->getLevel());
    for(int i=0;i<nodeRoot->getNbChildren();i++)
        inverseTree(nodeRoot->getChildren()[i]);
}
void sortVectorPixelRef(const int width,const int height ,const int caracteristic,const int* M,Node* Nodes,vector<int>& ListPixelReference){
    for (int i=0;i<width*height;i++){
        if (!count(ListPixelReference.begin(),ListPixelReference.end(),M[i]))
            ListPixelReference.push_back(M[i]);
    }
    if (caracteristic==0)
        sort(ListPixelReference.begin(),ListPixelReference.end(),[&Nodes](const auto& a,const auto& b){ return Nodes[a].getArea()< Nodes[b].getArea(); });
    else if (caracteristic==1)
        sort(ListPixelReference.begin(),ListPixelReference.end(),[&Nodes](const auto& a,const auto& b){ return Nodes[a].getHighest()< Nodes[b].getHighest(); });
    else
        sort(ListPixelReference.begin(),ListPixelReference.end(),[&Nodes](const auto& a,const auto& b){ return Nodes[a].getVol()< Nodes[b].getVol(); });

}

int numberLeaf(Node* nodeRoot){
    if (nodeRoot->getNbChildren()==0){
        return 1;}
    int L=0;
    for (int i=0;i<nodeRoot->getNbChildren();i++){
        L+=numberLeaf(nodeRoot->getChildren()[i]);
    }
    return L;
}
int firstLeaf(vector<int>& ListPixelWhichStay,Node* Nodes){
    while(Nodes[ListPixelWhichStay.front()].getNbChildren()!=0)
        ListPixelWhichStay.erase(ListPixelWhichStay.begin());
    int l=ListPixelWhichStay.front();
    ListPixelWhichStay.erase(ListPixelWhichStay.begin());
    return l;
}
int toPixelRef(Node* n, Node* Nodes,vector<int> ListPixelReference){
    for (int i=0;i<ListPixelReference.size();i++){
        if (Nodes[ListPixelReference[i]]==*n)
            return ListPixelReference[i];
    }
    return -1;
}
byte* Keep_N_Lobes (int* V,const int width,const int height,const int* M,Node* Nodes,Node* nodeRoot,int root,const int caracteristic,  const int N){
    vector<int> ListPixelReference={};
    sortVectorPixelRef(width,height,caracteristic,M,Nodes,ListPixelReference);
    vector<int> Q;
    int L=numberLeaf(nodeRoot);
    vector<int> ListPixelWhichStay(ListPixelReference.size());
    copy(ListPixelReference.begin(),ListPixelReference.end(),ListPixelWhichStay.begin());
    while(L>N){
        int c=firstLeaf(ListPixelWhichStay,Nodes);
        int p=toPixelRef(Nodes[c].getParent(),Nodes,ListPixelReference);
        Nodes[p].setNbChildren(Nodes[p].getNbChildren()-1);
        if (Nodes[p].getNbChildren()>0)
            L--;
        Nodes[c].setMark(1);
        Q.push_back(c);
    }
    Nodes[root].setMark(1);
    while (Q.size()!=0){
        int c=Q.front();
        Q.erase(Q.begin());
        RemoveLobe(c,Nodes,ListPixelReference);
    }
    byte* imageReconstruct=new byte[width*height];
    for(int i=0;i<width*height;i++){
        imageReconstruct[V[i]]=Nodes[M[V[i]]].getLevel();
    }
    return imageReconstruct;
}
int RemoveLobe(int c,Node* Nodes,vector<int>ListPixelReference){
    Node p=Nodes[c];
    if (*Nodes[c].getParent()==Nodes[c])
        return c;
    if (Nodes[c].getMark()==1)
        Nodes[c]=Nodes[RemoveLobe(toPixelRef(Nodes[c].getParent(),Nodes,ListPixelReference),Nodes,ListPixelReference)];
    return c;
}


Node* get_parent_commun(Node* n1,Node* n2){
    if (n1==n2)
        return n1;
    if (n1->getArea()==n2->getArea())
        get_parent_commun(n1->getParent(),n2);
    if (n1->getArea()<n2->getArea())
        get_parent_commun(n1->getParent(),n2);
    else
        get_parent_commun(n1,n2->getParent());
};
