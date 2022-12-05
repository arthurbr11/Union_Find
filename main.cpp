#include "tree.h"



const char* default_image_file=srcPath("/running-horse-rect.png");


int main(){
    string image_file = (argc>1)? argv[1]: default_image_file;
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl;
    if(! loadGreyImage(image_file, image, width, height)) {
        cerr << "Error loading image file " << image_file << endl;
        return 1;
    }
    int N=width*height;
    int* V=new int [N];
    int* ParNode= new int [N];
    int* RankNode=new int [N];
    int* ParTree=new int [N];
    int* RankTree=new int [N];
    int* M=new int [N];
    int* lowest_node=new int [N];
    Node* Nodes=new Node [N];
    for (int i=0;i<N;i++)
        V[i]=i;
    int Root=-1;
    BuildComponentTree(V,width,height,image,ParNode,RankNode,ParTree,RankTree,Nodes,Root,M,lowest_node);

    delete [] V;
    delete [] ParNode;
    delete [] ParTree;
    delete [] RankNode;
    delete [] RankTree;
    delete [] M;
    delete [] lowest_node;
    delete [] Nodes;
    delete default_image_file;



    return 0;
}
