/*************************************************************************
	> File Name: creat.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月30日 星期六 19时04分35秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> 

#define KH  0

typedef struct BiTNode
{ 
    char data;
    struct BiTNode *Lchild;
    struct BiTNode *Rchild;
} BiTNode, *BiTree;

BiTree Create(BiTree root)
{ 
	char ch;
	ch=getchar();
	if(ch == '#')
		root = KH;
	
	else
	{
		if(!(root=(BiTNode *)malloc(sizeof(BiTNode))))
			printf("Error!"); 
		root->data = ch;
		root->Lchild = Create(root->Lchild);
		root->Rchild = Create(root->Rchild);
	}
	return root;
}
void Preorder(BiTree root)
{
	if(root)
	{
		printf("%c", root->data);
		Preorder(root->Lchild);
		Preorder(root->Rchild);
	}
}
void InOrder(BiTree root)
{
	if(root)
	{
		InOrder(root->Lchild);
		printf("%c", root->data);
		InOrder(root->Rchild);
	}
}
void PostOrder(BiTree root)
{
	if(root)
	{
		PostOrder(root->Lchild);
		PostOrder(root->Rchild);
		printf("%c", root->data);
	}
}
int main()
{
	BiTree root;
	root=Create(root);
	Preorder(root);
	printf("\n");
	InOrder(root);
	printf("\n");
	PostOrder(root);
}






