/*************************************************************************
	> File Name: 建立与层次遍历.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月08日 星期五 08时17分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 30
#define datatype char

typedef struct Node {
    datatype  data;
    struct Node *Lchild;
    struct Node *Rchild;
} BiTNode;

BiTNode *Create()
{
    int flag = 0;
    BiTNode *root, *pnew, *pend;
    root = (BiTNode *)malloc(sizeof(BiTNode));
    scanf("%c", &root->data);
    root->Lchild = NULL;
    root->Rchild = NULL;
    return root;
}

void Visit(BiTNode root->data)
{
    
}

void PreOrder(BiTNode *root) 
{
    if(root != NULL) {
        Visit(root->data);
        PreOrder(root->Lchild);
        PreOrder(root->Rchild);
    }
}

void InOrder(BiTNode *root)
{
    if(root != NULL) {
        InOrder(root->data);
        Visit(root->Lchild);
        InOrder(root->Rchild);
    }
}

void PostOrder(BiTNode *root)
{
    if(root != NULL) {
        PostOrder(root->Lchild);
        PostOrder(root->Rchild);
        Visit(root->data);
    }
}

