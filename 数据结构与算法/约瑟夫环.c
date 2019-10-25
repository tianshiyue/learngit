/*************************************************************************
	> File Name: 约瑟夫环.c
	> Author: 
	> Mail: 
	> Created Time: 2019年10月23日 星期三 20时06分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    int ipassword;
    struct Node *next;
} Node;


Node *Creatlist(int);
void job(Node *, int, int);

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);
    // 创建单循环链表
    Node *phead;
    phead = Creatlist(n);
    job(phead, n, m);
}

// 创建有n个节点的单循环链表
Node *Creatlist(int n)
{
    Node *phead = NULL, *pnew, *pend;
    for(int i = 1; i <= n; i++) {
        pnew = (Node *)malloc(sizeof(Node));
        scanf("%d", &pnew->ipassword);
        pnew->id = i;
        if(phead == NULL) {
            phead = (Node *)malloc(sizeof(Node));
            phead->next = pnew;
            pend = phead;
        }
        pend->next = pnew;
        pend = pnew;
    }
    pend->next = phead->next;
    return phead;
}

void job(Node *phead, int n, int m)
{
    Node *ptemp, *qtemp;
    ptemp = qtemp = phead->next;
    int i = m;
    while(n > 0) {
        for(int j = 1; j < i; j++) {
            ptemp = ptemp->next;
        }
        while(qtemp->next != ptemp) {
            qtemp = qtemp->next;
        }
        qtemp->next = ptemp->next;
        i = ptemp->ipassword;
        printf("%-2d", ptemp->id);
        ptemp = ptemp->next;
        n--;
    }
}
