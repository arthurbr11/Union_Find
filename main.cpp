#include "tree.h"


const char* default_image_file=srcPath("/test-2.jpg");




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
    Node* nodeRoot =new Node ();
    *nodeRoot=Nodes[root];
    string prefix="-",  indent= " ";
    Node* n=new Node (-1);
    make_parent(nodeRoot,n);
    display( nodeRoot,prefix,  indent);
    display_parent(nodeRoot,prefix,  indent);

    cout <<numberLeaf(nodeRoot)<<endl;

    drawTree(image_rebuilt,nodeRoot);
    putGreyImage(IntPoint2(0,0), image_rebuilt, width, height);
    click();

    byte* imageReconstruct=Keep_N_Lobes(V,width,height,M,Nodes,nodeRoot,AREA,2);
    clearWindow();
    putGreyImage(IntPoint2(0,0), imageReconstruct, width, height);

    click();
    clearWindow();
    putGreyImage(IntPoint2(0,0), image, width, height);

    int i1,j1;
    int sw;
    int button1=anyGetMouse(j1,i1,window,sw);
    while(button1==1){
        draw_with_parent(&Nodes[M[i1*width+j1]],width,height);

        click();
        clearWindow();
        putGreyImage(IntPoint2(0,0), image, width, height);

        button1=anyGetMouse(j1,i1,window,sw);
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
