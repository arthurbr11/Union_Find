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
    /**********DISPLAY THE TREES *********************/

    string prefix="-",  indent= " ";
    cout<<" increasing"<<endl;
    display( nodeRoot_incr,prefix,  indent);
    cout<<" decreasing"<<endl;
    display( nodeRoot_decr,prefix,  indent);

    //    byte* imageReconstruct=Keep_N_Lobes(V,width,height,M,Nodes,nodeRoot,root,AREA,2);
    //    clearWindow();
    //    putGreyImage(IntPoint2(0,0), imageReconstruct, width, height);

    //    click();
    //    clearWindow();
    //    putGreyImage(IntPoint2(0,0), image, width, height);

    /**********DISPLAY UNDER WITH PARENTS IN GREEN (RIGHT CLICK DECREASING & LEFT CLICK INCREASING)  *********************/
    //    int i1,j1;
    //    int button1=getMouse(j1,i1);
    //    while(true){
    //        if (button1==1)
    //            draw_with_parent(&Nodes_decr[M_decr[i1*width+j1]],width,height);

    //        else
    //            draw_with_parent(&Nodes_incr[M_incr[i1*width+j1]],width,height);
    //        click();
    //        clearWindow();
    //        putGreyImage(IntPoint2(0,0), image, width, height);

    //        button1=getMouse(j1,i1);
    //    }
    /**********DISPLAY BETWEEN TWO POINT (RIGHT CLICK DECREASING & LEFT CLICK INCREASING)  *********************/

            int i1,j1,i2,j2;
            int button1=getMouse(j1,i1);
            int button2=getMouse(j2,i2);
            while(true){
                cout<<i1<<" "<<j1<<endl;
                cout<<i2<<" "<<j2<<endl;
                if (button1==1)
                    draw(get_parent_commun(&Nodes_decr[M_decr[i1*width+j1]],&Nodes_decr[M_decr[i2*width+j2]]),image,width,height);

                else
                    draw(get_parent_commun(&Nodes_incr[M_incr[i1*width+j1]],&Nodes_incr[M_incr[i2*width+j2]]),image,width,height);

                click();
                clearWindow();
                putGreyImage(IntPoint2(0,0), image, width, height);

                button1=getMouse(j1,i1);
                button2=getMouse(j2,i2);

            }
//    int i1,j1,i2,j2;
//    Event e;

//    while(true){
//        getEvent(0,e);
//        if (e.type==EVT_BUT_ON){
//            i1=e.pix[1],j1=e.pix[0];
//            cout<<i1<<" "<<j1<<endl;
//            while (e.type!=EVT_BUT_OFF){
//                if (e.type==EVT_MOTION){
//                    i2=e.pix[1],j2=e.pix[0];
//                    cout<<i1<<" "<<j1<<endl;
//                    cout<<i2<<" "<<j2<<endl;
//                    if (e.button==1)
//                        draw(get_parent_commun(&Nodes_decr[M_decr[i1*width+j1]],&Nodes_decr[M_decr[i2*width+j2]]),image,width,height);

//                    else
//                        draw(get_parent_commun(&Nodes_incr[M_incr[i1*width+j1]],&Nodes_incr[M_incr[i2*width+j2]]),image,width,height);

//                }
//                getEvent(0,e);
//            }
//            clearWindow();
//            putGreyImage(IntPoint2(0,0), image, width, height);
//        }
//    }

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
