#include "tree.h"


const char* default_image_file=srcPath("/running-horse-rect.png");


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
    //putGreyImage(IntPoint2(0,0), image, width, height);
    //endGraphics();


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

    int i1,j1,i2,j2;
    int sw;
    anyGetMouse(j1,i1,window,sw);
    int button2=anyGetMouse(j2,i2,window,sw);
    while(button2==1){
        Nodes[M[i1*width+j1]].getLevel();
        Nodes[M[i2*width+j2]].getLevel();

        for (int i=0;i<Nodes[M[i2*width+j2]].getPixel().size();i++){
            int p=Nodes[M[i2*width+j2]].getPixel()[i];
            drawPoint(p%width,p/width,RED);
        }

        button2=anyGetMouse(j2,i2,window,sw);
        clearWindow();
        putGreyImage(IntPoint2(0,0), image, width, height);
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
