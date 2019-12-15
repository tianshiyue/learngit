#include<stdio.h>
#include<string.h>
#include<malloc.h> 
#define KH  0
int sum=0;
typedef struct BiTNode
{ 
char data;
struct BiTNode *Lchild,*Rchild;
}BiTNode,*BiTree;
BiTree Create(BiTree root)
{ 
	char ch;
	ch=getchar();
	if(ch=='#')
		root=KH;
	
	else
	{
		if(!(root=(BiTNode *)malloc(sizeof(BiTNode))))
			printf("Error!"); 
		root->data=ch;
		root->Lchild=Create(root->Lchild);
		root->Rchild=Create(root->Rchild);
	}
	return root;
}

void InOrder(BiTree root)
{
	if(root)
	{
		InOrder(root->Lchild);
		if(root->Lchild==NULL&&root->Rchild==NULL)
		{
			printf("%c",root->data);
		}
		InOrder(root->Rchild);
	}
}
int leaf(BiTree root)
{
	int nl,nr;
	if(root==NULL)
	{
		return 0;
	}
	if((root->Lchild==NULL)&&(root->Rchild==NULL))
	{
		return 1;
	}
	nl=leaf(root->Lchild);
	nr=leaf(root->Rchild);
	return(nl+nr);
}
int DegreeTwo(BiTree root)
{
	
	if(root==NULL)
	{
		return 0;
	}
	if((root->Lchild!=NULL)&&(root->Rchild!=NULL))
	{
		return 1+DegreeTwo(root->Lchild)+DegreeTwo(root->Rchild);
	}


	return DegreeTwo(root->Lchild)+DegreeTwo(root->Rchild);
}
int DegreeOne(BiTree root)
{
	if(root==NULL)
	{
		return 0;
	}
	if((root->Lchild==NULL&&root->Rchild!=NULL)||(root->Lchild!=NULL&&root->Rchild==NULL))
	{
		sum++;
	}
	if(root->Lchild!=NULL)
	{
		DegreeOne(root->Lchild);
	}
	if(root->Rchild!=NULL)
	{
		DegreeOne(root->Rchild);
	}
	
}
int main()
{
	BiTree root;
	int m,n;
	root=Create(root);
	m=leaf(root);
	n=DegreeTwo(root);
	DegreeOne(root);
	printf("%d %d %d",m,n,sum);
	printf("\n");
	InOrder(root);
}

