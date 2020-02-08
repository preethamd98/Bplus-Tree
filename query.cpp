#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include "main.cpp"


using namespace std;

int pre_processor(string A,map<string,int> DDB,set<char> db){
    string temp;
    for(int i=0;i<A.length();i++){
        if(db.count(A[i])!=0){
            temp+=A[i];
        }
    }
    return DDB[temp];

}

void processor(BTree *T,string A,map<string,int> DDB,set<char> db)
{

   ofstream outfile;
   outfile.open("output.txt",ios_base::app);

    int a = pre_processor(A,DDB,db);
    int X;
    double Y;
    if(a==1){
        int i=A.length()-1;
        while(1){
            if(A[i]!='('){
                i--;
            }
            else{
                break;
            }
        }
        X = stoi(A.substr(i+1,A.length()-2));
        T->clear_order();
        T->set_order(X);
        
      
    }
    else if(a==2){
        int i=A.length()-1;
        int j=A.length()-1;
        while(1){if(A[i]!='('){i--;}else{break;}}
        while(1){if(A[j]!=','){j--;}else{break;}}
        X = stoi(A.substr(i+1,j-1));
        Y = stod(A.substr(j+1,A.length()-2));
        KVPair a(X,Y);
        T->insert(a);




    }
    else if(a==3){
        int i=A.length()-1;
        while(1){
            if(A[i]!='('){
                i--;
            }
            else{
                break;
            }
        }
        X = stoi(A.substr(i+1,A.length()-2));
        KVPair a(X,0);
       // T->delete_N(a);

    }
        else if(a==4){
        int i=A.length()-1;
        while(1){
            if(A[i]!='('){
                i--;
            }
            else{
                break;
            }
        }
        X = stoi(A.substr(i+1,A.length()-2));

        outfile<<T->search(X)<<endl;


    }
    else if(a==5){
        int i=A.length()-1;
        int j=A.length()-1;
        while(1){if(A[i]!='('){i--;}else{break;}}
        while(1){if(A[j]!=','){j--;}else{break;}}
        X = stoi(A.substr(i+1,j-1));
        Y = stod(A.substr(j+1,A.length()-2));
 
        outfile<<T->rsearch(X,Y)<<endl;



    }
    else{
        cout<<"Wrong Query!"<<endl;
    }
}




int main(int argc, char *argv[]){

    vector<string> A;
    string a;
    ifstream infile; 
    infile.open(argv[1]);
    infile>>a;
    //cout<<a<<endl;
    A.push_back(a);
    while(!infile.eof()){
        infile>>a;
        A.push_back(a);
    }

    infile.close();
    
    //Query Database

    vector<string> DB = {"Initialize()","Insert(,)","Delete()","Search()","Search(,)"};
    map<string,int> DDB;
    for(int i=0;i<DB.size();i++){
        DDB[DB[i]] = i+1;
    }
    set<char> db;
    for(int i=0;i<DB.size();i++){
        for(int j=0;j<DB[i].size();j++){
            db.insert(DB[i][j]);
        }
    }

   


    



   BTree *X = new BTree();

   for(int i=0;i<A.size();i++){
       processor(X,A[i],DDB,db);

   }

 
   







    return 0;



}