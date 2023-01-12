#include "tree.h"


const char* default_image_file=srcPath("/test-2.jpg");

byte* imageReverse(const int N,byte* image){
    byte* imageReverse=new byte[N];
    for(int i=0;i<N;i++){
        imageReverse[i]=255-image[i];
    }
    return imageReverse;
}


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

    int N=width*height;
    /**********COMPONENT TREE(DECREASING) FOR THE IMAGE *********************/
    int* V_decr=new int [N];
    Attributes* att_decr=new Attributes[N];
    int* M_decr=new int [N];
    int* lowest_node_decr=new int [N];
    Node* Nodes_decr=new Node [N];
    for (int i=0;i<N;i++)
        V_decr[i]=i;
    int root_decr=-1;
    Node* nodeRoot_decr =new Node ();


    BuildComponentTree(V_decr,width,height,image,att_decr,Nodes_decr,nodeRoot_decr,root_decr,M_decr,lowest_node_decr);

    /**********COMPONENT TREE(INCREASING) FOR THE IMAGE *********************/
    int* V_incr=new int [N];
    Attributes* att_incr=new Attributes[N];
    int* M_incr=new int [N];
    int* lowest_node_incr=new int [N];
    Node* Nodes_incr=new Node [N];
    for (int i=0;i<N;i++)
        V_incr[i]=i;
    int root_incr=-1;
    Node* nodeRoot_incr =new Node ();
    byte* image_incr=imageReverse(N,image);

    BuildComponentTree(V_incr,width,height,image_incr,att_incr,Nodes_incr,nodeRoot_incr,root_incr,M_incr,lowest_node_incr);
    inverseTree(nodeRoot_incr);

    string prefix="-",  indent= " ";
    display( nodeRoot_incr,prefix,  indent);
    display( nodeRoot_decr,prefix,  indent);

//    byte* imageReconstruct=Keep_N_Lobes(V,width,height,M,Nodes,nodeRoot,root,AREA,2);
//    clearWindow();
//    putGreyImage(IntPoint2(0,0), imageReconstruct, width, height);

//    click();
//    clearWindow();
//    putGreyImage(IntPoint2(0,0), image, width, height);
    int i1,j1;
    int sw;
    int button1=anyGetMouse(j1,i1,window,sw);
    while(true){
        if (button1==1)
            draw_with_parent(&Nodes_decr[M_decr[i1*width+j1]],width,height);
        else
            draw_with_parent(&Nodes_incr[M_incr[i1*width+j1]],width,height);
        click();
        clearWindow();
        putGreyImage(IntPoint2(0,0), image, width, height);

        button1=anyGetMouse(j1,i1,window,sw);
    }
    endGraphics();

    delete [] V_incr;
    delete [] att_incr;
    delete [] M_incr;
    delete [] lowest_node_incr;
    delete [] Nodes_incr;
    delete [] V_decr;
    delete [] att_decr;
    delete [] M_decr;
    delete [] lowest_node_decr;
    delete [] Nodes_decr;
    delete default_image_file;


    return 0;
}
