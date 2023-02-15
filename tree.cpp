#include "tree.h"


void MakeSet(const int x,Attributes* att,const bool type){
    att[x].setPar(x,type),att[x].setRank(0,type);
}
int Find(const int x,Attributes* att,const bool type){
    if(att[x].getPar(type)!=x)
        att[x].setPar(Find(att[x].getPar(type),att,type),type);
    return att[x].getPar(type);}
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
        int n=Levels[255-i].size();
        for(int j=0;j<n;j++){
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
void make_parent(Node* nodeRoot,Node* parent){
    nodeRoot->setParent(parent);
    for (int i=0;i<nodeRoot->getNbChildren();i++)
        make_parent(nodeRoot->getChildren()[i],nodeRoot);
};

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
        int n=valid_neighborhood.size();
        for(int j=0;j<n;j++){
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
    make_parent(&Nodes[root],&Nodes[root]);
    *nodeRoot=Nodes[root];
    computeVolume(nodeRoot);
}

int computeVolume(Node* n){
    int vol = n->getArea();
    if(n->getNbChildren()>0){
        int nbChildren=n->getNbChildren();
        for(int i=0;i<nbChildren;i++)
            vol += computeVolume(n->getChildren()[i])+n->getChildren()[i]->getArea()*(n->getChildren()[i]->getLevel()-n->getLevel());
    }
    n->setVol(vol);
    return vol;

}

void Pixel_under_n(vector<int> &vector_pixel,Node* n){
    int nbPixel=n->getPixel().size();
    for (int i=0;i<nbPixel;i++){
        vector_pixel.push_back(n->getPixel()[i]);
    }
    if (n->getNbChildren()==0)
        return ;
    int nbChildren=n->getNbChildren();
    for (int i=0;i<nbChildren;i++){
        Pixel_under_n(vector_pixel,n->getChildren()[i]);
    }
    return ;
}

void drawTree(byte* image,Node* nodeRoot){
    int nbChildren=nodeRoot->getNbChildren();
    for (int i=0;i<nbChildren;i++)
        drawTree(image,nodeRoot->getChildren()[i]);
    int nbPixel=nodeRoot->getPixel().size();
    for (int i=0;i<nbPixel;i++)
        image[nodeRoot->getPixel()[i]]=nodeRoot->getLevel();
}
void display(Node* nodeRoot,string prefix, string indent){
    cout << prefix<<nodeRoot->getLevel()<< endl;
    int nbChildren=nodeRoot->getNbChildren();
    for (int i=0;i<nbChildren;i++){
        display(nodeRoot->getChildren()[i],prefix+indent,indent);
    }
};
void draw(Node* n,const byte* F,const int width,const int height){
    vector<int> vetcor_pixel={};
    Pixel_under_n(vetcor_pixel,n);
    const int N=width*height;
    byte * r=new byte[N];
    byte * g=new byte[N];
    byte * b=new byte[N];

    for (int i=0;i<N;i++){
        r[i]=F[i];
        g[i]=F[i];
        b[i]=F[i];
    }
    int nbPixel=vetcor_pixel.size();
    for (int i=0;i<nbPixel;i++){
        int p=vetcor_pixel[i];
        g[p]=0;
        b[p]=0;
        r[p]=255;
    }
    noRefreshBegin();
    putColorImage(IntPoint2(0,0), r,g,b, width, height);
    noRefreshEnd();
}
void draw_with_parent(Node* n,const byte* F,const int width,const int height){
    draw(n,F,width,height);
    int nbPixel=n->getPixel().size();
    for (int i=0;i<nbPixel;i++){
        int p=n->getPixel()[i];
        drawPoint(p%width,p/width,GREEN);
    }
}
void display_tree_terminal(Node* nodeRoot_incr, Node* nodeRoot_decr){
    string prefix="-",  indent= " ";
    cout<<" increasing"<<endl;
    display( nodeRoot_incr,prefix,  indent);
    cout<<" decreasing"<<endl;
    display( nodeRoot_decr,prefix,  indent);
}

byte* imageReverse(const int N,byte* image){
    byte* imageReverse=new byte[N];
    for(int i=0;i<N;i++){
        imageReverse[i]=255-image[i];
    }
    return imageReverse;
}
void inverseTree(Node* nodeRoot){
    nodeRoot->setLevel(255-nodeRoot->getLevel());
    int nbChildren=nodeRoot->getNbChildren();
    for(int i=0;i<nbChildren;i++)
        inverseTree(nodeRoot->getChildren()[i]);
}
void sortVectorPixelRef(const int width,const int height ,const int caracteristic,const int* M,Node* Nodes,vector<int>& ListPixelReference){
    const int N=width*height;
    for (int i=0;i<N;i++){
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
    int nbChildren=nodeRoot->getNbChildren();
    for (int i=0;i<nbChildren;i++){
        L+=numberLeaf(nodeRoot->getChildren()[i]);
    }
    return L;
}
int firstLeaf(vector<int>& ListPixelWhichStay,Node* Nodes){
    int i=0;
    while(Nodes[ListPixelWhichStay[i]].getNbChildren()!=0)
        i++;
    int leaf=ListPixelWhichStay[i];
    ListPixelWhichStay.erase(ListPixelWhichStay.begin()+i);
    return leaf;
}
int toPixelRef(Node* n, Node* Nodes,const vector<int> ListPixelReference){
    int nbPix=ListPixelReference.size();
    for (int i=0;i<nbPix;i++){
        if (Nodes[ListPixelReference[i]]==*n)
            return ListPixelReference[i];
    }
    return -1;}
Node* get_parent_commun(Node* n1,Node* n2){
    if (n1==n2 and n1->getConsideration() and n2->getConsideration())
        return n1;
    if (n1->getArea()==n2->getArea())
        get_parent_commun(n1->getParent(),n2);
    if (n1->getArea()<n2->getArea())
        get_parent_commun(n1->getParent(),n2);
    else
        get_parent_commun(n1,n2->getParent());
};

void display_node_children(Node* Nodes_incr, int* M_incr, Node* Nodes_decr, int* M_decr,const byte* image,const int width,const int height){
    int i1,j1;
    int button1=getMouse(j1,i1);
    while(true){
        if (button1==1)
            draw_with_parent(&Nodes_decr[M_decr[i1*width+j1]],image,width,height);
        else
            draw_with_parent(&Nodes_incr[M_incr[i1*width+j1]],image,width,height);
        click();
        clearWindow();
        putGreyImage(IntPoint2(0,0), image, width, height);

        button1=getMouse(j1,i1);
    }
}
void display_two_clicks(Node* Nodes_incr, int* M_incr, Node* Nodes_decr, int* M_decr,const byte* image,const int width,const int height){
    int i1,j1,i2,j2;
    int button1=getMouse(j1,i1);
    int button2=getMouse(j2,i2);
    while(true){
        if (button1==1)
            draw(get_parent_commun(&Nodes_decr[M_decr[i1*width+j1]],&Nodes_decr[M_decr[i2*width+j2]]),image,width,height);

        else
            draw(get_parent_commun(&Nodes_incr[M_incr[i1*width+j1]],&Nodes_incr[M_incr[i2*width+j2]]),image,width,height);

        click();
        clearWindow();
        putGreyImage(IntPoint2(0,0), image, width, height);

        button1=getMouse(j1,i1);
        button2=getMouse(j2,i2);

    }}
void display_keep_clicking(Node* Nodes_incr, int* M_incr, Node* Nodes_decr, int* M_decr,const byte* image,const int width,const int height){
    int i1,j1,i2,j2;  //(i1,j1) : first click || (i2,j2) : position of the mouse
    Event e;

    while(true){
        getEvent(1,e);
        if (e.type==EVT_BUT_ON){ //first click
            i1=e.pix[1],j1=e.pix[0];
            int click_type = e.button;
            while (e.type!=EVT_BUT_OFF){
                getEvent(-1,e);
                if (e.type==EVT_MOTION){ //mouse is changing of pixel
                    i2=e.pix[1],j2=e.pix[0];
                    if (click_type==1)
                        draw(get_parent_commun(&Nodes_decr[M_decr[i1*width+j1]],&Nodes_decr[M_decr[i2*width+j2]]),image,width,height);
                    else if (click_type==3)
                        draw(get_parent_commun(&Nodes_incr[M_incr[i1*width+j1]],&Nodes_incr[M_incr[i2*width+j2]]),image,width,height);
                }
            }
            clearWindow();
            putGreyImage(IntPoint2(0,0), image, width, height);
        }
    }
}

void filter_tree(Node* n,const int treshold,const int level){
    //If we are in the nodes who are being blurred -> change the role of the node in the tree to false
    int nbChildren=n->getNbChildren();
    if(level!=-1){
        n->setConsideration(false);
        for(int i=0; i<nbChildren; i++)
            filter_tree(n->getChildren()[i], treshold, level);
    }
    else{
        //Else, we don't change the role of the node in the tree
        //If area>treshold : we keep going
        if(n->getArea()>treshold){
            for(int i=0; i<nbChildren; i++)
                filter_tree(n->getChildren()[i],treshold, level);
        }
        //Else we modify the levels of the children to be the node's one (=blurring)
        else{
            for(int i=0; i<nbChildren; i++)
                filter_tree(n->getChildren()[i], treshold, n->getLevel());
        }
    }
}
int level_consider(Node* node){
    if (node->getConsideration())
        return node->getLevel();
    return level_consider(node->getParent());
}
void filter_picture(Node* nodeRoot,byte* new_image,const byte* image){
    int nbPixel=nodeRoot->getPixel().size();
    if (nodeRoot->getConsideration()){
        for (int i=0;i<nbPixel;i++)
            new_image[nodeRoot->getPixel()[i]]=image[nodeRoot->getPixel()[i]];
    }
    else{
        int level=level_consider(nodeRoot);
        for (int i=0;i<nbPixel;i++)
            new_image[nodeRoot->getPixel()[i]]=level;
    }
    int nbChildren=nodeRoot->getNbChildren();
    for (int i=0;i<nbChildren;i++){
        filter_picture(nodeRoot->getChildren()[i],new_image,image);
    }

}
void display_filtered_picture(Node* nodeRoot,const byte* image,const int treshold,const int width,const int height){
    byte* new_image = new byte[width*height];
    filter_tree(nodeRoot, treshold, -1);
    filter_picture(nodeRoot,new_image,image);
    clearWindow();
    putGreyImage(IntPoint2(0,0), new_image, width, height);
}

//After this part the code doesn't work.We didn't manage to finish


byte* Keep_N_Lobes (int* V,const int width,const int height,const int* M,Node* Nodes,Node* nodeRoot,int root,const int caracteristic,  const int N){
    vector<int> ListPixelReference={};
    sortVectorPixelRef(width,height,caracteristic,M,Nodes,ListPixelReference);
    vector<int> Q={};
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


