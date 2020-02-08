#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <vector>
#include <list>
#include <stack>
#include <string>

using namespace std;


//Key value pair as a structure
struct KVPair{
    int key;
    double value;
    KVPair(){}
    KVPair(int a,double b){
        key = a;
        value = b;
    }
};

//Function for sorting the pointers in the node
bool myfunction (struct KVPair i,struct KVPair j) { return (i.key<j.key); }


//Every node either a leaf,element or the root node come under the class name BNode
class BNode{
    public:
    int degree;
    bool root;
    bool children;
    bool leaf;
    bool element;
    vector<KVPair> keys; //List of key value pairs in the node
    vector<BNode*> pointers; //List of child pointers in the node

    BNode *LEFT; //Pointer to the Left sibling
    BNode *RIGHT; //Pointer to the right sibling  
    BNode *parent; //Pointer to the parent node
    BNode(){}// NULL constructor

    BNode(int m){ // Parameterized contructor with taking order as an argument
        degree = m;
        children = 0;
        leaf = 0;
        LEFT = NULL;
        RIGHT = NULL;
        parent = NULL;
    }

    

        //Helper function which is useful to return the pointer child node by taking the key as an input
        BNode* getPointer(int key){
        if(key<keys[0].key){
            return pointers[0];
            
        }
        else if(key>=keys[keys.size()-1].key){
            return pointers[keys.size()];
            
        }
        else{
            for(int i=0;i<keys.size()-1;i++){
                if(key>=keys[i].key && key<keys[i+1].key){
                    return pointers[i+1];
                }
            }
        }
        return NULL;

    }

    //Helper function used while debugging

    void pprint(){
        for(int i=0;i<keys.size();i++){
            cout<<keys[i].key<<","<<keys[i].value<<"   "<<endl;
        }
        cout<<endl;
       
    }

    //Check if the the node is full or not     
    bool isfull(){
        if(keys.size()==degree){
            return 1;
        }
        else{
            return 0;
        }
    }

    //Inserting a key value pair in the node and sorting the elements
    void insert_element(KVPair a){
            keys.push_back(a);
            sort(keys.begin(),keys.end(),myfunction);
            

}
    //Inserting a pointer into a node
    void insert_pointer(BNode *a){
        pointers.push_back(a);
       
    }

    
    void makeRoot(){root = 1;element = 0;leaf = 0;} // Make the current node as a root node
    void makeLeaf(){leaf = 1;root = 0;element = 0;} // Make the current  node as a leaf node
    void makeElement(){ element = 1;leaf = 0;root = 0;} // Make the current node as an element node
    bool isRoot(){return root;} //Check if the current node is a root node or not
    bool hasChildren(){return children;} //Check if the current node has children or not
    bool isElement(){return element;} //Check if the current node is the element node or not
    bool isLeaf(){return leaf;}  //Check if the current node is a leaf node or not
};  


//Function used inside the sort function to sort the elements according to their keys.
bool sorted_ptr (BNode* a, BNode* b) {if(a!=NULL && b!=NULL){return (a->keys[0].key<b->keys[0].key);} else{return 0;}}



//BTree class 
class BTree{
    public: 
    int order;
    BNode *root;
    BTree(){}
    BTree(int order){// Constructor used to intialise the tree and allocate memory for the root node
        this->order = order;
        root = new BNode(order);
        root->isRoot();
    }

    void clear_order(){// Useful function while processing the queries this is used it the tree has to be instialised more than once at the time of processing of queries
        root = NULL;
        this->order=0;

}
    void set_order(int order){// Set the order of the tree when the constructor is not used
        this->order = order;
        root = new BNode(order);
        root->isRoot();
    }

   
    void tester(KVPair x){ // Testing function used at the time of debugging
        BNode *temp = root;
         while(temp->pointers.size()!=0){temp = temp->getPointer(x.key);}
       
         while(temp!=NULL){
             for(int i=0;i<temp->keys.size();i++){
                 cout<<temp->keys[i].key;
             }
             
             temp = temp->RIGHT;
            
         }
    }



        string search(int k){ // Takes in the element that you want to search a key value and returns the string which can be used to written in the file
        BNode *temp = root;
         while(temp->pointers.size()!=0){temp = temp->getPointer(k);}
         int found = 0; 
         string A = "Null";
         while(temp!=NULL){
             for(int i=0;i<temp->keys.size();i++){
                 if(temp->keys[i].key==k){A = to_string(temp->keys[i].value);found=1;}
             }
             if(!found){temp = temp->RIGHT;}
             else{break;}
             
            
         }
         return A;

    }



    string rsearch(int k1,int k2){ // Takes in a range of values and then returns the values of the the keys within the range as a string which can be used to write inside a file

        BNode *temp = root;
        while(temp->pointers.size()!=0){temp = temp->getPointer(k1);}
        string A = "";
        while(temp!=NULL){
             for(int i=0;i<temp->keys.size();i++){
                 if(temp->keys[i].key>=k1 && temp->keys[i].key<=k2){A+=to_string(temp->keys[i].value)+",";}
             }
            temp = temp->RIGHT;
        }
        if(A==""){return "Null";}
        else{A.pop_back();
        return A;}





    }
    void non_leaf_split(BNode *a){// This function is used at the time of insertion when the non leaf node has to be splitted
        BNode* temp1 = a->parent;
        int split_index = floor(order/2);
        if(temp1!=NULL){
            //Create two new nodes to copy the values from the current node
            BNode* A = new BNode(order);
            BNode* B = new BNode(order);
            for(int i=0;i<split_index;i++){
                A->insert_element(a->keys[i]);
            }
            for(int i=split_index + 1;i<order;i++){
                B->insert_element(a->keys[i]);

            }
            for(int i=0;i<=split_index;i++){
                A->insert_pointer(a->pointers[i]);
            }
            for(int i=split_index+1;i<order+1;i++){
                B->insert_pointer(a->pointers[i]);
            }
            //Inserting the middle element inside the parent node
            temp1->insert_element(a->keys[split_index]);

            //Search for the pointer to the current node and replace the pointer with the newly created node

            for(int i=0;i<temp1->pointers.size();i++){
                if(temp1->pointers[i]==a){
                    temp1->pointers[i]=A;
                }

            }
            //Change the parent pointers of the newly created nodes

            for (int i = 0; i < B->pointers.size(); i++){
                B->pointers[i]->parent = B;
            }
            for (int i = 0; i < A->pointers.size(); i++){
                A->pointers[i]->parent = A;
            }
            sort(A->pointers.begin(),A->pointers.end(),sorted_ptr);
            sort(B->pointers.begin(),B->pointers.end(),sorted_ptr);

            
            temp1->insert_pointer(B);
            
            A->parent = temp1;
            B->parent = temp1;

            //Sort the pointers based on the first element of the child
            sort(temp1->pointers.begin(),temp1->pointers.end(),sorted_ptr);
        
            //Readjust the leaf node pointers so that the doubly linked list property is preserved
            
            for(int i = 0; i < A->pointers.size() - 1; i++){
            
                A->pointers[i+1]->LEFT = A->pointers[i];
                A->pointers[i]->RIGHT = A->pointers[i+1];
          
            }
            for(int i = 0; i < B->pointers.size() - 1; i++){
                B->pointers[i+1]->LEFT = B->pointers[i];
                B->pointers[i]->RIGHT = B->pointers[i+1];
            }
          
            A->pointers[A->pointers.size()-1]->RIGHT = B->pointers[0];
            B->pointers[0]->LEFT = A->pointers[A->pointers.size()-1];
           

        }
        //if the current node is a non leaf and a parent node the parent node has to be splitted into three nodes and the values, pointers has to be readjusted
        else if(temp1==NULL){
            

            //Allocation of memory to the new nodes.
            BNode* A = new BNode(order);
            BNode* B = new BNode(order);
            BNode* C = new BNode(order);
            
            //making B the parent node
            B->insert_pointer(A);
            B->insert_pointer(C);
            A->parent = B;
            C->parent = B;
            int split_index = floor(order/2);//index for the rearrangement of pointers

            B->insert_element(a->keys[split_index]);

            for(int i=0;i<split_index;i++){
                A->insert_element(a->keys[i]);
            }
            for(int i=split_index+1;i<order;i++){
                C->insert_element(a->keys[i]);
            }

            //Readjusting the pointers
            for(int i=0;i<=split_index;i++){
                A->insert_pointer(a->pointers[i]);
            }
            for(int i=split_index+1;i<=order;i++){
                C->insert_pointer(a->pointers[i]);
            }
            for (int i = 0; i < C->pointers.size(); i++){
                C->pointers[i]->parent = C;
            }
            for (int i = 0; i < A->pointers.size(); i++){
                A->pointers[i]->parent = A;
            }
            //Sorting of the pointers to maintain the order
            sort(B->pointers.begin(),B->pointers.end(),sorted_ptr);
            sort(A->pointers.begin(),A->pointers.end(),sorted_ptr);
            sort(C->pointers.begin(),C->pointers.end(),sorted_ptr);
            for(int i = 0; i < A->pointers.size() - 1; i++){
                A->pointers[i+1]->LEFT = A->pointers[i];
                A->pointers[i]->RIGHT = A->pointers[i+1];
      
            }

            for(int i = 0; i < C->pointers.size() - 1; i++){
                C->pointers[i+1]->LEFT = C->pointers[i];
                C->pointers[i]->RIGHT = C->pointers[i+1];
            }
            
            A->pointers[A->pointers.size()-1]->RIGHT = C->pointers[0];
            C->pointers[0]->LEFT = A->pointers[A->pointers.size()-1];
            root = B;//Creating the new root
}
}


//Helper function useful during a leaf_split
    void leaf_split(BNode *a){
        BNode *temp = a->parent;
  

        int split_index = floor(order/2);
        if(temp!=NULL){
          
            BNode* A = new BNode(order);
            BNode* B = new BNode(order);
            for(int i=0;i<split_index;i++){
                A->insert_element(a->keys[i]);

            }
            for(int i=split_index;i<order;i++){
                B->insert_element(a->keys[i]);

            }

            
            temp->insert_element(a->keys[split_index]);

            for(int i=0;i<temp->pointers.size();i++){
                
                if(temp->pointers[i]==a){
                    temp->pointers[i]=A;
                }

            }

           
     
            temp->insert_pointer(B);
          
            
            A->parent = temp;
            B->parent = temp;   

            BNode *temp_left = a->LEFT;
            BNode *temp_right = a->RIGHT;
            

            A->RIGHT = B;
            A->LEFT = temp_left;
            B->LEFT = A;

            
            sort(temp->pointers.begin(),temp->pointers.end(),sorted_ptr);
            sort(B->pointers.begin(),B->pointers.end(),sorted_ptr);
            sort(A->pointers.begin(),A->pointers.end(),sorted_ptr);
            
        

        }
        else if(a->parent==NULL){
          
            BNode* A = new BNode(order);
            BNode* B = new BNode(order);
            BNode* C = new BNode(order);
            B->insert_pointer(A);
            B->insert_pointer(C);
            A->parent = B;
            C->parent = B;

            A->RIGHT = C;
            C->LEFT = A;


            int split_index = floor(order/2);
            
            B->insert_element(a->keys[split_index]);
            
            for(int i=0;i<split_index;i++){
                A->insert_element(a->keys[i]);
            }
            
            for(int i=split_index;i<order;i++){
                C->insert_element(a->keys[i]);
            }
            sort(A->pointers.begin(),A->pointers.end(),sorted_ptr);
            sort(C->pointers.begin(),C->pointers.end(),sorted_ptr);
            
            sort(B->pointers.begin(),B->pointers.end(),sorted_ptr);

            root = B;
            
}
}




//Helper function used for debugging

    BNode* getLeaf(BNode* root,KVPair x){
        BNode *temp = root;
        while(!temp->isLeaf()){temp = temp->getPointer(x.key);}
        return temp;
    }



//Insert function for a tree
    void insert(KVPair x){
        BNode *temp = root;

        while(temp->pointers.size()!=0){temp = temp->getPointer(x.key);}

        temp->insert_element(x);
               
        
        while(temp!=NULL){
            if(temp->pointers.size()==0){
                if(temp->isfull()){leaf_split(temp);
                }
            }
            else if(temp->pointers.size()!=0){
                if(temp->isfull()){
                    non_leaf_split(temp);
                }
            }
            temp=temp->parent;

            
            
        }
    }




    // Deleting the element in a B+ tree and 
    void delete_N(KVPair x){
        BNode *temp = root;
        //Moving towards the leaf to delete the actual key-value pair
        while(temp->pointers.size()!=0){temp = temp->getPointer(x.key);}
        
        if(temp->pointers.size()==0){
            int index = -1;
            for(int i=0;i<temp->keys.size();i++){
                if(x.key==temp->keys[i].key){
                    index = i;
                }
            }
            //Deleting the Key-Value pair as soon as reaching the leaf node
            temp->keys.erase(temp->keys.begin()+index);
          
            int parent_index = -1;
            for(int i=0;i<temp->parent->keys.size()+1;i++){
                if(temp->parent->pointers[i]==temp){
                    parent_index = i;
                }
            }
            


            if(temp->pointers.size()==0 && temp->parent!=NULL){
              
            if((temp->keys.size()+1)>=ceil(order/2.0) && temp->parent!=NULL){
            
            if(index>0){}
            else if(index==0){
                temp->parent->keys[parent_index-1] = temp->keys[0];
               
            }}
           
            
            else if(temp->keys.size()+1<ceil(order/2.0) && temp->parent!=NULL){
                
            if(temp->LEFT!=NULL && temp->LEFT->parent==temp->parent){
                if(temp->LEFT->keys.size()+1>ceil(order/2.0) && temp->LEFT!=NULL){
                    
                    
                    KVPair k1 = temp->LEFT->keys[temp->LEFT->keys.size()-1];
                    KVPair k2 = temp->keys[0];

                    temp->LEFT->keys.erase(temp->LEFT->keys.end()-1);
                    temp->insert_element(k1);
                    

                    for(int i=0;i<temp->parent->keys.size();i++){
                        if(temp->parent->keys[i].key == k2.key){
                            temp->parent->keys[i] = k1;
                        }
                    }
                }
            }
            else if(temp->RIGHT!=NULL && temp->RIGHT->parent==temp->parent){
                if(temp->RIGHT->keys.size()+1>ceil(order/2.0) && temp->RIGHT!=NULL){
                    KVPair t1 = temp->RIGHT->keys[0];
                    KVPair t2 = temp->RIGHT->keys[1];
            
                    temp->insert_element(t1);
                    temp->RIGHT->keys.erase(temp->RIGHT->keys.begin());
                    for(int i=0;i<temp->parent->keys.size();i++){
                        if(temp->parent->keys[i].key == t1.key){
                            temp->parent->keys[i] = t2;
                        }
                    }
                }
            }
            //Merging of leaves

            if(temp->LEFT!=NULL && temp->LEFT->parent==temp->parent){
                
                if((temp->LEFT->keys.size()+temp->keys.size()+1)>ceil(order/2.0)){
                    BNode *current_pt = temp;
                    BNode *left = temp->LEFT;
                    for(int i=0;i<temp->LEFT->keys.size();i++){
                        current_pt->insert_element(temp->LEFT->keys[i]);
                    }

                    temp->LEFT = temp->LEFT->LEFT;
                    temp->LEFT->RIGHT = temp;


                    for(int i=0;i<current_pt->parent->pointers.size();i++){
                        if(current_pt->parent->pointers[i]==current_pt){
                            current_pt->parent->pointers.erase(current_pt->parent->pointers.begin()+i);
                            if(temp->parent->pointers[i+1]==NULL){
                            
                                temp->parent->keys.erase(temp->parent->keys.begin()+i);
                            
                                
                            }
                            else if(temp->parent->keys[i+1].key==temp->parent->pointers[i+1]->keys[0].key){
                                temp->parent->keys.erase(temp->parent->keys.begin()+i);
                            }
                            else{
                                temp->parent->keys[i] = temp->RIGHT->keys[0];

                            }



                            
                            
                        }
                    }



                }
            }
            else if(temp->RIGHT!=NULL && temp->RIGHT->parent==temp->parent){
                 if((temp->LEFT->keys.size()+temp->keys.size()+1)>ceil(order/2.0)){
                    BNode *current_pt = temp;
                    BNode *right = temp->RIGHT;
                    for(int i=0;i<temp->RIGHT->keys.size();i++){
                        current_pt->insert_element(temp->RIGHT->keys[i]);
                    }

                    temp->RIGHT = temp->RIGHT->RIGHT;
                    temp->RIGHT->LEFT = temp;


                    for(int i=0;i<current_pt->parent->pointers.size();i++){
                        if(current_pt->parent->pointers[i]==current_pt){
                            current_pt->parent->pointers.erase(current_pt->parent->pointers.begin()+i);
                            if(temp->parent->pointers[i+1]==NULL){
                            temp->parent->keys.erase(temp->parent->keys.begin()+i);
                            
                                
                            }
    
                        }
                    }



                }
             }





            }

        BNode *X = temp;
        while(X!=NULL){
            if(temp->parent==NULL){
                int index = -1;
                if(temp->keys.size()==0){
                    root = temp->pointers[0];
                    temp->pointers[0]->parent = NULL;
                }
                for(int i=0;i<temp->parent->keys.size();i++){
                    if(temp->parent->keys[i].key==x.key){
                        index = i;
                        temp->parent->keys.erase(temp->parent->keys.begin()+i);
                        BNode* temp2 = temp;
                        while(temp2!=NULL){
                            temp2=temp2->pointers[0];
                        }
                        KVPair t = temp2->keys[0];
                        temp->insert_element(t);
                        break;
                    }
                }
                break;
            }
            else if(temp->pointers.size()!=0){
                //internal node
                for(int i=0;i<temp->keys.size();i++){
                    if(temp->keys[i].key==x.key){
                        temp->keys.erase(temp->keys.begin()+i);
                        break;
                    }
                }
                    
                if(temp->keys.size()+1>=ceil(order/2)){
                    temp = temp->parent;
                    continue;

                }
                else{
                    int cn = temp->pointers.size();
                    if(cn==0){
                        //Redistribute
                        if(temp->RIGHT!=NULL && temp->RIGHT->parent==temp->parent){
                            //RIGHT redistribution
                            if((temp->RIGHT->keys.size()+temp->keys.size())>ceil(order/2.0)){
                                BNode *current_pt = temp;
                                BNode *right = temp->RIGHT;
                                for(int i=0;i<right->keys.size();i++){
                                    current_pt->insert_element(right->keys[i]);
                                }
                                temp->RIGHT = temp->RIGHT->RIGHT;
                                temp->RIGHT->LEFT = temp;
                                for(int i=0;i<current_pt->parent->pointers.size();i++){
                                    if(current_pt->parent->pointers[i]==current_pt){
                                        current_pt->parent->pointers.erase(current_pt->parent->pointers.begin()+i);
                                        current_pt->parent->keys.erase(current_pt->parent->keys.begin()+i-1);
                                    }
                                }
                                //redistribute pointers
                            }
                            BNode *a = temp->RIGHT->pointers[0];
                            temp->RIGHT->pointers.erase(temp->RIGHT->pointers.begin());
                            temp->insert_pointer(a);

                        }
                        else if(temp->LEFT!=NULL && temp->LEFT->parent==temp->parent){
                            //LEFT redistribution
                            if((temp->LEFT->keys.size()+temp->keys.size())>ceil(order/2.0)){
                                BNode *current_pt = temp;
                                BNode *right = temp->LEFT;
                                for(int i=0;i<right->keys.size();i++){
                                    current_pt->insert_element(right->keys[i]);
                                }
                                temp->LEFT = temp->LEFT->LEFT;
                                temp->LEFT->RIGHT = temp;
                                for(int i=0;i<current_pt->parent->pointers.size();i++){
                                    if(current_pt->parent->pointers[i]==current_pt){
                                        current_pt->parent->pointers.erase(current_pt->parent->pointers.begin()+i);
                                        current_pt->parent->keys.erase(current_pt->parent->keys.begin()+i-1);
                                    }
                                }
                                //redistribute pointers
                            BNode *a = temp->LEFT->pointers[temp->LEFT->pointers.size()-1];
                            temp->LEFT->pointers.erase(temp->LEFT->pointers.end()-1);
                            temp->insert_pointer(a);




                            }
                        }
                        //Merge
            
            


                    }
                    else{
                        BNode *left_sibling;
                        BNode *right_sibling;
                        index = -1;
                        for(int i=0;i<temp->parent->pointers.size();i++){
                            if(temp->parent->pointers[i]==temp){
                                index = i;
                            }

                        }
                        //Redistribute
                        if(index==0){left_sibling=NULL;right_sibling=temp->parent->pointers[1];}
                        else if(index==temp->parent->pointers.size()-1){
                            right_sibling=NULL;
                            left_sibling = temp->parent->pointers[temp->parent->pointers.size()-2];
                        }
                        else{
                            right_sibling=temp->parent->pointers[index+1];
                            left_sibling = temp->parent->pointers[index-1];
                        }

                        if(left_sibling!=NULL && left_sibling->keys.size()+1>=ceil(order/2.0)){
                            KVPair a = left_sibling->keys[left_sibling->keys.size()-1];
                            temp->insert_element(a);
                            temp->parent->keys[index-1] = a;
                        }
                        else if(right_sibling!=NULL && right_sibling->keys.size()+1>=ceil(order/2.0)){
                            KVPair a = right_sibling->keys[right_sibling->keys.size()-1];
                            temp->insert_element(a);
                            temp->parent->keys[index+1] = a;
                        }
                        //pointers redistributed

                        //Merge
                        //Left merge
                        ///Right Merge

                        X = X->parent;
                    }
                }           
            }
        }
}




        
            


       if(temp->pointers.size()!=0 && temp->parent!=NULL){
            KVPair r = x;
            int index=-1;
            for(int i=0;i<temp->keys.size();i++){
                if(temp->keys[i].key==x.key){
                    index = i;
                }
            }
            if(index==-1){return;}
            else{           
            temp->keys.erase(temp->keys.begin()+index);
            if(temp->keys.size()>=ceil((order-1)/2.0)){
                return;
            }
            else if(temp->keys.size()<ceil((order-1)/2.0)){
                BNode *left_sibling;
                BNode *right_sibling;
                index = -1;
                for(int i=0;i<temp->parent->pointers.size();i++){
                    if(temp->parent->pointers[i]==temp){
                        index = i;
                    }

                }
                if(index==0){left_sibling=NULL;right_sibling=temp->parent->pointers[1];}
                else if(index==temp->parent->pointers.size()-1){
                    right_sibling=NULL;
                    left_sibling = temp->parent->pointers[temp->parent->pointers.size()-2];
                }
                else{
                    right_sibling=temp->parent->pointers[index+1];
                    left_sibling = temp->parent->pointers[index-1];
                }

                if(left_sibling!=NULL && left_sibling->keys.size()+1>=ceil(order/2.0)){
                    KVPair a = left_sibling->keys[left_sibling->keys.size()-1];
                    temp->insert_element(a);
                    temp->parent->keys[index-1] = a;
                }
                else if(right_sibling!=NULL && right_sibling->keys.size()+1>=ceil(order/2.0)){
                    KVPair a = right_sibling->keys[right_sibling->keys.size()-1];
                    temp->insert_element(a);
                    temp->parent->keys[index+1] = a;
                }
            }
        }
     }
    }
 }
 
 };


