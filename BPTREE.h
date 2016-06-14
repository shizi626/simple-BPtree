#ifndef BPTREE_H_INCLUDED
#define BPTREE_H_INCLUDED

#include <string>
using namespace std;

class Bplustree
{
    //this is a 4-5 tree
const static int Max_Number_Of_Branches=5;

private:
    Bplustree *branch[Max_Number_Of_Branches]; //for storing branches
    int key[Max_Number_Of_Branches-1]; //for storing the key in the node
    int keyNumNow;  //the number of key in the node now

    //pointer to the right sibling
    //have to do some converting
    //when one node is creating
    //and is connected to other nodes
    Bplustree *Sequential_Next;

    //the father of the node
    //have to do some converting when the one branch is moved from one node to another
    //otherwise it will lead to some error
    Bplustree *father;

    bool leaf;//whether the node if leaf or not

    void Refresh(int x,Bplustree *p);
    void InsertIndex(Bplustree *p,int x,Bplustree *s,Bplustree *q,Bplustree *&root);
    void DeleteIndex(Bplustree *p,int k,Bplustree *&root);

public:
    Bplustree(){
        for (int i=0;i<Max_Number_Of_Branches;i++)
            this->branch[i]=NULL;
        for (int i=0;i<Max_Number_Of_Branches-1;i++)
            this->key[i]=0;
        this->keyNumNow=0;
        this->father=NULL;
        this->Sequential_Next=NULL;
        this->leaf=true;
    }
    ~Bplustree(){
    }

    static Bplustree *FindLeaf(int x,Bplustree *root);
    int Insert(int x,Bplustree *&root);
    bool Search(int x,Bplustree *root);
    void Delete(int x,Bplustree *&root);
    void print(Bplustree* root);
};


#endif // BPTREE_H_INCLUDED
