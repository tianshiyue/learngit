/*************************************************************************
	> File Name: 迷宫.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月29日 星期日 19时41分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 51

typedef struct Node {
    int x;
    int y;
    struct Node *next;
} Node;

Node *top;

// 初始化栈
Node * seqstack_init()
{
    Node *top;
    top = (Node *)malloc(sizeof(Node));
    top->next = NULL;
    return top;
}
void pull(Node *top, int x, int y)
{
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->x = x;
    p->y = y;
    p->next = top->next;
    top->next = p;
}

// 删除栈顶元素
int push(Node *top)
{
    if(top->next == NULL) {
        return 0;
    }
    Node *ptemp;
    ptemp = top->next;
    top->next = ptemp->next;
    free(ptemp);
    return 1;
}

int Gettop(Node *top, int *x, int *y)
{
    if(top->next == NULL) {
        return 0;
    }
    Node *ptemp;
    ptemp = top->next;
    *x = ptemp->x;
    *y = ptemp->y;
    return 1;
}

// 越界判断
int Check(int *x, int *y)
{
    if(*x >= 0 && *x <= 4 && *y >= 0 && *y <= 4) {
        return 1;
    }
    return 0;
}

int search_map(Node *top,int (*map)[4],  int *tx, int *ty)
{
    // 判断是否越界
    int oo = Check(tx, ty);
    if(oo) {
        int x, y;
        // 得栈顶元素
        int flag = Gettop(top, &x, &y);
        if(!flag) {
            return 0;
        }
        *tx = x;
        *ty = y + 1;
        if(map[*tx][*ty] == 0) {
            return 1;
        }
        *tx = x - 1;
        *ty = y;
        if(map[*tx][*ty] == 0) {
            return 1;
        }
        *tx = x;
        *ty = y - 1;
        if(map[*tx][*ty] == 0) {
            return 1;
        }
        *tx = x + 1;
        *ty = y;
        if(map[*tx][*ty] == 0) {
            return 1;
        }
    }
    return 0;
}

int main()
{
    Node *top;
    top = seqstack_init();
    int book[2][2];
    int map[4][4] = {{1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {1, 1, 0, 0},
                 {0, 0, 0, 1}};
    int x, y;
    int flag = 1;
    // 输入起点
    printf("input strating:");
    scanf("%d, %d", &book[0][0], &book[0][1]);
    // 输入终点
    printf("input ending:");
    scanf("%d, %d", &book[1][0], &book[1][1]);
    // 容错
    if(map[book[0][0]][book[0][1]] == 1) {
        printf("starting is obstacle\n");
        return 0;
    } 
    if(map[book[1][0]][book[1][1]] == 1) {
        printf("ending is obstacle\n");
        return 0;
    }
    if(!Check(&book[0][0], &book[0][1])) {
        printf("starting input error\n");
        return 0;
    }
    if(!Check(&book[1][0], &book[1][1])) {
        printf("ending input error\n");
        return 0;
    }
    // 将迷宫起点入栈
    pull(top, book[0][0], book[0][1]);
    x = book[0][0];
    y = book[0][1];
    // 当栈顶元素等于终点时 即可以出去 打印路径
    while((x != book[1][0]) || (y != book[1][1])) {
        if(flag == 0) {
            printf("sorry don'have exit\n");
            return 0;
        }
        // 取栈顶元素
        if(Gettop(top, &x, &y) == 0) {
            return 0;
        }
        // 将走过的点置为1 标志不能再走
        map[x][y] = 1;
        // 调用走迷宫函数 看此时周围是否能走
        flag = search_map(top, map, &x, &y);
        if(flag == 1) {
            pull(top, x, y);
        } else {
            push(top);
        }
        flag = Gettop(top, &x, &y);
    }
    while(flag != 0) {
        printf("path:(%d, %d)  \n", x, y);
        push(top);
        flag = Gettop(top, &x, &y);
    }
    return 0;
}
