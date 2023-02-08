#include "tree.h"

const char* default_image_file=srcPath("/tableau-2.jpg");

byte* imageReverse(const int N,byte* image){
    byte* imageReverse=new byte[N];
    for(int i=0;i<N;i++){
        imageReverse[i]=255-image[i];
    }
    return imageReverse;
}


int main(){

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

    /**********DISPLAY THE TREES *********************/

    //display_tree_terminal(nodeRoot_incr, nodeRoot_decr);
//       byte* imageReconstruct=Keep_N_Lobes(V_incr,width,height,M_incr,Nodes_incr,nodeRoot_incr,root_incr,AREA,10);
//       clearWindow();
//       putGreyImage(IntPoint2(0,0), imageReconstruct, width, height);

//       click();
//       clearWindow();
//       putGreyImage(IntPoint2(0,0), image, width, height);

    /**********DISPLAY UNDER WITH PARENTS IN GREEN (RIGHT CLICK DECREASING & LEFT CLICK INCREASING)  *********************/


    //display_node_children(Nodes_incr, M_incr,Nodes_decr, M_decr, image, width, height);


    /**********DISPLAY BETWEEN TWO POINT (RIGHT CLICK DECREASING & LEFT CLICK INCREASING)  *********************/


    //display_two_clicks(Nodes_incr, M_incr,Nodes_decr, M_decr, image, width, height);
    filter_tree(Nodes_decr,500,-1);
    filter_tree(Nodes_incr,500,-1);

    display_keep_clicking(Nodes_incr, M_incr,Nodes_decr, M_decr, image, width, height);

    display_filtered_picture(nodeRoot_incr, image,50, width, height);
    cout<<"treshold = 50"<<endl;
    click();
    display_filtered_picture(nodeRoot_incr, image,100, width, height);
    cout<<"treshold =100"<<endl;
    click();
    display_filtered_picture(nodeRoot_incr, image,1000, width, height);
    cout<<"treshold = 1000"<<endl;
    click();
    display_filtered_picture(nodeRoot_incr, image,10000, width, height);
    cout<<"treshold = 10000"<<endl;
    click();



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
