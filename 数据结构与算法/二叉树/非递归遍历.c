/*************************************************************************
	> File Name: 非递归遍历.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月30日 星期六 20时46分03秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h> 

#define KH  0
#define datatype char
#define MAX 100

typedef struct BiTNode
{ 
    datatype  data;
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

typedef struct 
{
    int top;
    BiTree* elem[MAX]; 
} Stacktree;

void InitStack(Stacktree *S)
{
    S->top = 0;
}
void Push(Stacktree* S, BiTree* x)
{
    int top1 = S -> top;
    if (x->data == '#')
    {
        return;
    }
    else
    {
        S->elem[top1++] = x;
        S->top++;
    }
}

int Pop(Stacktree *S)
{
    int top = S->top;
    if (S->top == 0)
    {
        return 0;
    }
    else
    {
        --(S->top);
        return 1;
    }
}

BiTree* GetTop(Stacktree *S)
{
    int top1 = S->top;
    BiTree *p;
    p = S->elem[top1--];
    return p;
}

BiTree* GetTop1(Stacktree *S)
{
    int top1 = S->top;
    BiTree*p;
    top1--;
    p = S->elem[top1];
    return p;
}

int IsEmpty(Stacktree *S)
{
    return(S->top == 0 ? 1 : 0);
}



void PreOrder(BiTree *p)
{
    if(p)
    {
        Stacktree tree ;
        InitStack(&tree);
        BiTree *root = p;
        while(root != NULL || !IsEmpty(&tree))
        {
            while(root != NULL)
            {
                printf("%c", root->data);
                Push(&tree, root);
                root = root->Lchild;
            }

            if(!IsEmpty(&tree))
            {
                Pop(&tree);
                root = GetTop(&tree);
                root = root->Rchild;
            }
        }
   }
}

void inorder(BiTree *p)
{
    if(p)
    {
        Stacktree tree;
        InitStack(&tree);
        BiTree *root = p;
        while(root != NULL || !IsEmpty(&tree))
        {
            while(root != NULL)
            {
                Push(&tree, root);
                root = root -> lchild;
            }

            if(!IsEmpty(&tree))
            {
                Pop(&tree);
                root = GetTop(&tree);
                printf("%c", root -> data);
                root = root -> rchild;
            }
        }
    }
}

void postorder(BiTree *p)
{
    Stacktree tree;
    InitStack(&tree);

    BiTree *root;    
    BiTree *pre = NULL;    
    Push(&tree, p);
    while (!IsEmpty(&tree))
    {
        root = GetTop1(&tree);
       if ((root -> lchild == NULL && root -> rchild == NULL) || (pre != NULL && (pre == root -> lchild || pre == root -> rchild)))
        {
            printf("%c", root -> data);
            Pop(&tree);
            pre = root;
        }

        else
        {
            if (root -> rchild != NULL)
            {
                Push(&tree, root -> rchild);
            }

            if (root -> lchild != NULL)
            {
                Push(&tree, root -> lchild);
            }
        }

    }
}

int main()
{ 
    BiTree *root = createTree();
    PreOrder(root);
    printf("\n");
    inorder(root);
    printf("\n");
    postorder(root);
}


