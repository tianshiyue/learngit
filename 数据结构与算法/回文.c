/*************************************************************************
	> File Name: 回文.c
	> Author: 
	> Mail: 
	> Created Time: 2019年10月25日 星期五 20时25分50秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 50

typedef struct Node {
    char a[MAXSIZE];
    int top;
} Node;


Node *Init()
{
    Node *s;
    s = (Node *)malloc(sizeof(Node)); 
    s->top = -1;
    return s;
}

void push(Node *s, char c)
{
    if(s->top == MAXSIZE - 1) {
        printf("栈满\n");
        exit(-1);
    } else {
        s->top++;
        s->a[s->top] = c;
    }
}

int main(int argc, char *argv[])
{
    char d[MAXSIZE];;
    int next;
    scanf("%s", d);
    int len = strlen(d);
    int mid = len/2 - 1;
    Node *s;
    s = Init();
    for(int i = 0; i <= mid; i++) {
        push(s, d[i]);
    } 
    if(len%2 == 0) {
         next = mid + 1;
    } else {
        next = mid + 2;
    }
    for(int i = next; i < len; i++) {
        if(d[i] !=  s->a[s->top]) {
            break;
        }
        s->top--;
    }
    if(s->top == -1) {
        printf("yes\n");
    } else {
        printf("no\n");
    }
}
