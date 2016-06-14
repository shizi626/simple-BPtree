#include <iostream>
#include <string>
#include <stdlib.h>
//#include <random>
//#include <ctime>
#include "BPTREE.h"
using namespace std;

int main()
{
    int arrayNum=329;
    Bplustree *root=NULL;
    //srand(time(0));
    int array[arrayNum]={0};
    for(int i=0;i<arrayNum;++i){
        array[i]=i;
        root->Insert(array[i],root);
    }
    for(int i=0;i<arrayNum/2;++i){
        root->Delete(rand()%arrayNum,root);
    }
    for(int i=0;i<arrayNum;++i){
        if(root->Search(i,root))
            cout<<"search for "<<i<<" successfully!"<<endl;
        else
            cout<<"search for "<<i<<" in vain!"<<endl;
    }
    cout<<endl;
    root->print(root);

    return 0;
}
