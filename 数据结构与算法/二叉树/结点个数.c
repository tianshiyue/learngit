/*************************************************************************
	> File Name: 结点个数.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月30日 星期六 19时39分20秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h> 

#define datatype char
#define H  0 

int sum=0;

typedef struct BiTNode
{ 
    datatype data;
    struct BiTNode *Lchild;
    struct BiTNode *Rchild;
} BiTNode, *BiTree;

BiTree Create(BiTree root)
{ 
	char ch;
	ch = getchar();
    if(ch == '#') {
        root = H;
    } else {
        if(!(root = (BiTNode *)malloc(sizeof(BiTNode)))){
            printf("error\n") ;
        }
		root->data = ch;
		root->Lchild = Create(root->Lchild);
		root->Rchild = Create(root->Rchild);
	}
	return root;
}

void Ptint(BiTree root)
{
	if(root) {
		Ptint(root->Lchild);
		if(root->Lchild==NULL && root->Rchild==NULL) {
			printf("%c",root->data);
		}
		Ptint(root->Rchild);
	}
}

// 叶子结点的个数
int leaf(BiTree root)
{
	int nl, nr;
	if(root == NULL) {
		return 0;
	}
	if((root->Lchild == NULL) && (root->Rchild == NULL)) {
		return 1;
	}
	nl = leaf(root->Lchild);
	nr = leaf(root->Rchild);
	return(nl+nr);
}

// 度为二的结点
int degreetwo(BiTree root)
{
	
	if(root == NULL) {
		return 0;
	}
	if((root->Lchild != NULL) && (root->Rchild != NULL)) {
		return 1 + degreetwo(root->Lchild) + degreetwo(root->Rchild);
	}
	return degreetwo(root->Lchild) + degreetwo(root->Rchild);
}

// 度为1
int degreeone(BiTree root)
{
	if(root==NULL) {
		return 0;
	}
	if((root->Lchild == NULL && root->Rchild != NULL)||(root->Lchild != NULL && root->Rchild == NULL)) {
		sum++;
	}
	if(root->Lchild != NULL) {
		degreeone(root->Lchild);
	}
	if(root->Rchild != NULL) {
		degreeone(root->Rchild);
	}
	
}
int main()
{
	BiTree root;
	int m,n;
	root = Create(root);
	m = leaf(root);
	n = degreetwo(root);
	degreeone(root);
	printf("%d %d %d", m, n, sum);
	printf("\n");
	Ptint(root);
}

