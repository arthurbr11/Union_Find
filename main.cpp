#include "tree.h"



const char* default_image_file=srcPath("/image_noir_et_blanc.jpg");


int main(){

    true? cout<<"s",cout<<"e":cout<<"f",cout<<"j";
    string image_file = (argc>1)? argv[1]: default_image_file;
    byte* image;
    int width, height;
    cout << "Loading image: " << image_file << endl;
    if(! loadGreyImage(image_file, image, width, height)) {
        cerr << "Error loading image file " << image_file << endl;
        return 1;
    }

    Window window = openWindow(width, height);
    putGreyImage(IntPoint2(0,0), image, width, height);
    endGraphics();

    int N=width*height;
    int* V=new int [N];
    Attributes* att=new Attributes[N];
    int* M=new int [N];
    int* lowest_node=new int [N];
    Node* Nodes=new Node [N];
    for (int i=0;i<N;i++)
        V[i]=i;
    int root=-1;
    BuildComponentTree(V,width,height,image,att,Nodes,root,M,lowest_node);

    for (int i=0;i<N;i++){
        Nodes[lowest_node[M[i]]].addPixel(i);
    }
    byte* image_rebuilt=new byte[N];
    Node* n1 =new Node ();
    *n1=Nodes[root];
    drawTree(image_rebuilt,n1);
    int histo[256],histo_rebuilt[256];
    for (int i=0;i<256;i++)
        histo[i]=0,histo_rebuilt[i]=0;
    for (int i=0;i<N;i++){
        histo[image[i]]+=1;
        histo_rebuilt[image_rebuilt[i]]+=1;
    }
    int dif=0;
    for (int i=0;i<256;i++){
        if (i!=0)
            dif+=abs(histo[i]-histo_rebuilt[i]);
        cout << "Level "<< i<< " there is "<< histo[i]<< " pixel in image and "<< histo_rebuilt[i]<< " pixel in image rebuilt"<<endl;
    }
    cout << "the  absolute diff is "<< dif << " and there is "<< histo_rebuilt[0]-histo[0]<< " pixel black more";
    putGreyImage(IntPoint2(0,0), image_rebuilt, width, height);


    endGraphics();

    delete [] V;
    delete [] att;
    delete [] M;
    delete [] lowest_node;
    delete [] Nodes;
    delete default_image_file;



    return 0;
}
