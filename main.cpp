#include "tree.h"


const char* default_image_file=srcPath("/test.jpg");


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
        Nodes[M[i]].addPixel(i);
    }
    byte* image_rebuilt=new byte[N];
    Node* n1 =new Node ();
    *n1=Nodes[root];
    string prefix="-",  indent= " ";
    //display( n1,prefix,  indent);
    drawTree(image_rebuilt,n1);
    putGreyImage(IntPoint2(0,0), image_rebuilt, width, height);


    IntPoint2 cord,cord2;
    int sw;
    int button=anyGetMouse(cord,window,sw);
    int button2=anyGetMouse(cord2,window,sw);
    while(button2==1){

    }
    endGraphics();

    delete [] V;
    delete [] att;
    delete [] M;
    delete [] lowest_node;
    delete [] Nodes;
    delete default_image_file;


    return 0;
}
