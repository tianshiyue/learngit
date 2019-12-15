/*************************************************************************
	> File Name: 结点个数2.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月30日 星期六 20时04分25秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef char datatype;

typedef struct Node
{
    datatype data;
    struct Node *lchild;
    struct Node *rchild;
}BiTNode, *BiTree;

BiTree creat()
{
    char ch;
    ch = getchar();
    if(ch == '#') {
        return NULL;
    }
    else {
        BiTree root = malloc(sizeof(BiTNode));
        root->data = ch;
        root -> lchild = creat();
        root -> rchild = creat();
        return root;
    }
}

int a = 0,b = 0,c = 0;
char s[10000];
int k = 0;

void print(BiTree root)
{
    if(root) {
        if(root->lchild == NULL && root->rchild == NULL) {
            s[k++] = root->data;
            a++;
        }
        if((root->lchild == NULL && root->rchild != NULL) || (root->lchild != NULL && root->rchild == NULL) ) {
            b++;
        }
        if(root->lchild != NULL && root->rchild !=NULL) {
            c++;
        }
        print(root->lchild);
        print(root->rchild);
    }
}

void PreOrder(BiTree root)
{
    if(root) {
        printf("%c",root->data);
        PreOrder(root->lchild);
        PreOrder(root->rchild);
    }  
}

int main()
{
    BiTree root;
    root = creat();
    print(root);
    printf("%d %d %d\n",a,b,c);
    for(int i = 0; i < k; i++)
    printf("%c",s[i]);
    return 0;
}

