#include "BPTREE.h"
#include <iostream>

using namespace std;

void Bplustree::print(Bplustree* root){
    //print the tree's leaves in order
    Bplustree* temp=Bplustree::FindLeaf(0,root);
    while(temp!=NULL){
        for(int i=0;i<temp->keyNumNow;++i){
            cout<<temp->key[i]<<' ';
        }
        cout<<endl;
        temp=temp->Sequential_Next;
    }
}

Bplustree* Bplustree::FindLeaf(int x,Bplustree *root)
{
    //find the node that contains the key
    //input:
    //x,the key wanted;
    //root,the root of the BPtree
    //output:the node that contains the key
    Bplustree *p=root;
    while (!p->leaf)
    {
        int j=p->keyNumNow;
        for (int i=0;i<p->keyNumNow;i++)
            if (x<p->key[i])
            {
                j=i;
                break;
            }
        p=p->branch[j];
    }
    return p;
}

bool Bplustree::Search(int x,Bplustree *root)
{
    //find the key wanted
    //input:
    //x,the key wanted
    //root, the root of the BPtree
    //output: true if the key is found in tree, otherwise, false
    Bplustree *p=FindLeaf(x,root);
    for (int i=0;i<p->keyNumNow;i++)
        if (p->key[i]==x)
            return true;
    return false;
}

void Bplustree::Refresh(int x,Bplustree *p)
{
    //change the inner node's keys to appropriate value
    //input:
    //x: the key that has been changed in one node
    //p: the node that contains the changed key
    Bplustree *q=p->father;
    Bplustree *r=p;

    while (q!=NULL)
    {
        int j;
        if (x==r->key[0] && q->branch[0]!=r)
        {
            //the modified node is not the left most child of q
            //and the modified key is the smallest of the node

            for (int i=0;i<q->keyNumNow;i++)//find the branch
                if (q->branch[i+1]==r)
                {
                    j=i;
                    break;   //break for
                }
        }
        else
            break;   //break while
        q->key[j]=x;
        r=q;
        q=q->father; //update up to the root of the tree
    }
    return;
}

int Bplustree::Insert(int x,Bplustree *&root)
{
    //insert a key into BPtree
    //input:
    //x: the key to be inserted
    //root: the root of the BPtree
    //output: true for successful insertion, false for failed one
    if (root==NULL)
    {
        //no tree at all
        root=new Bplustree;
        root->keyNumNow=1;
        root->key[0]=x;
        return 0;
    }
    Bplustree *p=FindLeaf(x,root);
    int j=p->keyNumNow;
    for (int i=0;i<p->keyNumNow;i++)
    {
        if (p->key[i]==x)
            return 1;
        if (p->key[i]>x)
        {
            //record the location of the first key bigger than x
            j=i;
            break;
        }
    }
    if (p->keyNumNow<Max_Number_Of_Branches-1)
    {
        //not exceed the max number of keys
        //move the keys bigger than x backward one step
        for (int i=p->keyNumNow;i>j;i--)
            p->key[i]=p->key[i-1];

        //insert x
        p->key[j]=x;
        p->keyNumNow++;
        Refresh(x,p);
        return 0;
    }

    int TempForSplit[Max_Number_Of_Branches],top=0;
    //record x and all the keys in the spliting node
    for (int i=0;i<j;i++)
        TempForSplit[top++]=p->key[i];
    TempForSplit[top++]=x;
    for (int i=j;i<Max_Number_Of_Branches-1;i++)
        TempForSplit[top++]=p->key[i];

    Bplustree *q=new Bplustree;
    q->Sequential_Next=p->Sequential_Next;
    p->Sequential_Next=q;

    p->keyNumNow=0;   //set the key number 0 for the operation below
    //allocate keys
    //half to p
    //half to q(one less than p if the number of keys is odd)
    for (int i=0;i<=(top-1)>>1;i++)
        p->key[p->keyNumNow++]=TempForSplit[i];
    for (int i=(top+1)>>1;i<top;i++)
        q->key[q->keyNumNow++]=TempForSplit[i];

    //node q now is not in BPtree, now insert q
    InsertIndex(p->father,q->key[0],p,q,root);
    return 0;
}

void Bplustree::InsertIndex(Bplustree *dad,int x,Bplustree *s,Bplustree *newnode,Bplustree *&root)
{
    //insert a splited node into the BPtree
    //input:
    //dad: the father node to accommodate a new branch
    //x: the new node's smallest key
    //s: the new node's left sibling
    //newnode: the new node to be inserted

    while (1)   //iterating until finish split
    {
        if (dad==NULL)
        {
            //no father node at all, just allocate branch 0 and branch 1
            Bplustree *r=new Bplustree;
            r->leaf=false;
            r->keyNumNow=1;
            r->key[0]=x;

            r->branch[0]=s;
            s->father=r;

            r->branch[1]=newnode;
            newnode->father=r;

            s->Sequential_Next=newnode;

            root=r;
            return;
        }

        int j=dad->keyNumNow;
        for (int i=0;i<dad->keyNumNow;i++)
            if (dad->key[i]>x)
            {
                j=i;
                break;
            }
        if (dad->keyNumNow<Max_Number_Of_Branches-1)
        {   //the father has enough room for a new key
            for (int i=dad->keyNumNow;i>j;i--)
            {
                dad->key[i]=dad->key[i-1];
                dad->branch[i+1]=dad->branch[i];
            }
            dad->key[j]=x;
            dad->keyNumNow++;

            dad->branch[j+1]=newnode;
            newnode->father=dad;

            //s is not necessarily used in this case
            //because s is already in its right position
            return;
        }

        int TempForSplit[Max_Number_Of_Branches];
        int top=0;
        for (int i=0;i<j;i++)
            TempForSplit[top++]=dad->key[i];
        TempForSplit[top++]=x;
        for (int i=j;i<Max_Number_Of_Branches-1;i++)
            TempForSplit[top++]=dad->key[i];

        dad->keyNumNow=0;

        //dad node split into two part
        //dad itself and pp
        Bplustree *pp=new Bplustree;
        pp->leaf=false;

        //connect pp with dad and the node behind
        pp->Sequential_Next=dad->Sequential_Next;
        dad->Sequential_Next=pp;

        for (int i=0;i<(top-1)>>1;i++){
            dad->key[dad->keyNumNow++]=TempForSplit[i];   //dad obtain the first half of keys
        }
        //in doo-even tree, the middle key is not allocated now
        for (int i=(top+1)>>1;i<top;i++){   //pp obtain the second half
            pp->key[pp->keyNumNow++]=TempForSplit[i];
        }

        Bplustree *temp=dad->branch[0];

        //the branches of dad node are concatenated before
        //now allocating them to these two nodes in sequence
        for(int i=0;i<(Max_Number_Of_Branches+1)>>1;i++){
            //firstly dad
            dad->branch[i]=temp;
            dad->branch[i]->father=dad;

            temp=temp->Sequential_Next;
        }
        //if the number of branches is odd, then the middle branch is allocated to dad
        if (Max_Number_Of_Branches%2==0)
        {
            dad->branch[dad->keyNumNow]=temp;
            dad->branch[dad->keyNumNow]->father=dad;

            temp=temp->Sequential_Next;
        }

        for(int i=0;i<(Max_Number_Of_Branches+1)>>1;i++){
            //then pp
            pp->branch[i]=temp;
            pp->branch[i]->father=pp;

            temp=temp->Sequential_Next;
        }

        //construct parameter for iteration
        s=dad;
        newnode=pp;
        dad=dad->father;
        x=TempForSplit[(top-1)>>1];   //the middle key is pushed up
    }
    return;
}

void Bplustree::Delete(int x,Bplustree *&root)
{
    Bplustree *delnode=FindLeaf(x,root);
    int j;
    for (int i=0;i<delnode->keyNumNow;i++)
        if (delnode->key[i]==x){//find the key to be deleted
            j=i;
            break;
        }

    for (int i=j;i<delnode->keyNumNow-1;i++)
        delnode->key[i]=delnode->key[i+1];  //delete the key in key array
    delnode->keyNumNow--;

    if (delnode==root || delnode->keyNumNow>=(Max_Number_Of_Branches-1)>>1)//no need for merge
        return;

    Bplustree *delDad=delnode->father;
    for (int i=0;i<=delDad->keyNumNow;i++)
        if (delDad->branch[i]==delnode)
        {
            //find branch number of delnode
            j=i;
            break;
        }

    Bplustree *sibling;

    if (j>0)
    {   //delnode is not the lest most node
        sibling=delDad->branch[j-1];
        if (sibling->keyNumNow>(Max_Number_Of_Branches-1)>>1)
        {
            //borrow a key and a branch from its left sibling
            for (int i=delnode->keyNumNow;i>0;i--)
                delnode->key[i]=delnode->key[i-1];
            delnode->keyNumNow++;
            delnode->key[0]=sibling->key[sibling->keyNumNow-1];
            sibling->keyNumNow--;
            Refresh(delnode->key[0],delnode);
            return;
        }

        //left sibling has not enough key for borrowing
        //merge delnode with sibling
        //and delete delnode
        for (int i=0;i<delnode->keyNumNow;i++)
            sibling->key[sibling->keyNumNow++]=delnode->key[i];

        sibling->Sequential_Next=delnode->Sequential_Next;
        delnode->Sequential_Next=NULL;
        delete delnode;

        //delete inner node in BPtree
        //j: the branch number
        DeleteIndex(delDad,j-1,root);
        return;
    }
    else
    {
        //similar to if block
        sibling=delDad->branch[j+1];
        if (sibling->keyNumNow>(Max_Number_Of_Branches-1)>>1)
        {
            delnode->key[delnode->keyNumNow++]=sibling->key[0];
            sibling->keyNumNow--;
            for (int i=0;i<sibling->keyNumNow;i++)
                sibling->key[i]=sibling->key[i+1];
            Refresh(sibling->key[0],sibling);
            return;
        }

        for (int i=0;i<sibling->keyNumNow;i++)
            delnode->key[delnode->keyNumNow++]=sibling->key[i];

        delnode->Sequential_Next=sibling->Sequential_Next;
        sibling->Sequential_Next=NULL;
        delete sibling;
        DeleteIndex(delDad,j,root);
        return;
    }
}

void Bplustree::DeleteIndex(Bplustree *delnode,int k,Bplustree *&root)
{
    //delete inner node in BPtree

    for (int i=k+1;i<delnode->keyNumNow;i++)
    {
        delnode->key[i-1]=delnode->key[i];
        delnode->branch[i]=delnode->branch[i+1];
    }
    delnode->keyNumNow--;
    while (1)
    {
        if (delnode->keyNumNow==0)
        {
            //it's root with no key
            //set the child of the node root
            Bplustree *r=delnode->branch[0];
            r->father=NULL;

            delete delnode;
            delnode=NULL;
            root=r;
            return;
        }

        //no need for merge
        if (delnode==root || delnode->keyNumNow>=(Max_Number_Of_Branches-1)>>1)
            return;

        int j;
        Bplustree *dad=delnode->father;
        for (int i=0;i<=dad->keyNumNow;i++)
            if (dad->branch[i]==delnode)//get the branch number of the delnode
            {
                j=i;
                break;
            }

        Bplustree *sibling;
        //similar way as general delete do with something more complex
        if (j>0)
        {
            sibling=dad->branch[j-1];
            if (sibling->keyNumNow>(Max_Number_Of_Branches-1)>>1)
            {
                for (int i=delnode->keyNumNow;i>0;i--)
                {
                    //shift delnode right one position for father node's key and left sibling's branch
                    delnode->key[i]=delnode->key[i-1];
                    delnode->branch[i+1]=delnode->branch[i];
                }
                delnode->branch[1]=delnode->branch[0];
                delnode->keyNumNow++;
                delnode->key[0]=dad->key[j-1];

                delnode->branch[0]=sibling->branch[sibling->keyNumNow];
                delnode->branch[0]->father=delnode;

                dad->key[j-1]=sibling->key[--sibling->keyNumNow];
                return;
            }

            //left sibling does not have enough keys
            //starting merging between left sibling and delnode

            sibling->key[sibling->keyNumNow++]=dad->key[j-1];//pull father node's key that is between left sibling and delnode

            sibling->branch[sibling->keyNumNow]=delnode->branch[0];
            sibling->branch[sibling->keyNumNow]->father=sibling;

            for (int i=0;i<delnode->keyNumNow;i++)
            {
                sibling->key[sibling->keyNumNow++]=delnode->key[i];

                sibling->branch[sibling->keyNumNow]=delnode->branch[i+1];
                sibling->branch[sibling->keyNumNow]->father=sibling;
            }

            //deleting
            sibling->Sequential_Next=delnode->Sequential_Next;
            delnode->Sequential_Next=NULL;
            delete delnode;
            delnode=NULL;

            for (int i=j;i<dad->keyNumNow;i++)
            {
                //modifying father node
                dad->key[i-1]=dad->key[i];
                dad->branch[i]=dad->branch[i+1];
            }
            dad->keyNumNow--;
            //recursively delete inner node
            delnode=dad;
            continue;
        }
        else
        {
            sibling=dad->branch[j+1];
            if (sibling->keyNumNow>(Max_Number_Of_Branches-1)>>1)
            {
                delnode->key[delnode->keyNumNow++]=dad->key[j];

                delnode->branch[delnode->keyNumNow]=sibling->branch[0];
                delnode->branch[delnode->keyNumNow]->father=delnode;

                dad->key[j]=sibling->key[0];
                sibling->keyNumNow--;
                for (int i=0;i<sibling->keyNumNow;i++)
                {
                    sibling->key[i]=sibling->key[i+1];
                    sibling->branch[i]=sibling->branch[i+1];
                }
                sibling->branch[sibling->keyNumNow]=sibling->branch[sibling->keyNumNow+1];
                return;
            }

            delnode->key[delnode->keyNumNow++]=dad->key[j];

            delnode->branch[delnode->keyNumNow]=sibling->branch[0];
            delnode->branch[delnode->keyNumNow]->father=delnode;

            for (int i=0;i<sibling->keyNumNow;i++)
            {
                delnode->key[delnode->keyNumNow++]=sibling->key[i];

                delnode->branch[delnode->keyNumNow]=sibling->branch[i+1];
                delnode->branch[delnode->keyNumNow]->father=delnode;

            }

            delnode->Sequential_Next=sibling->Sequential_Next;
            sibling->Sequential_Next=NULL;
            delete sibling;
            sibling=NULL;

            for (int i=j+1;i<dad->keyNumNow;i++)
            {
                dad->key[i-1]=dad->key[i];
                dad->branch[i]=dad->branch[i+1];
            }
            dad->keyNumNow--;
            delnode=dad;
            continue;
        }
    }
    return;
}
