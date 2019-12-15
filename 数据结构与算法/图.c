/*************************************************************************
	> File Name: 图.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月11日 星期三 19时05分00秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

typedef struct SqStack {
	int rear;
	int front;
	int vex[MAX];
} SqQueue;

typedef struct {
	int visited[MAX];
	int arcs[MAX][MAX];
	int arcnum;
	int vexnum;
	char vex[MAX];
} AdjMatrix; 
void Create(AdjMatrix *G)
{ 
	char Vex1, Vex2;
	int i, j, k, vex1, vex2;
	scanf("%d %d",&G->vexnum, &G->arcnum);
	for(i = 1; i <= G->vexnum; i++) {
		for(j = 1; j <= G->vexnum; j++) {
			G->arcs[i][j] = 0;
		}
	}
	getchar();
	for(i = 1; i <= G->vexnum; i++) {
		scanf("%c",&G->vex[i]);
	}
	getchar();
	for(k = 0; k < G->arcnum; k++) {
		scanf("%c", &Vex1);
		vex1 = Vex1-64;
		scanf("%c", &Vex2);
		vex2 = Vex2-64;
		getchar();
		G->arcs[vex1][vex2] = 1;
	}
}

void InitiaLise(AdjMatrix *G)
{
    int i;
	for(i = 1; i <= G->vexnum; i++) {
		G->visited[i] = 0;
	} 
}
void DFS(AdjMatrix *G,int v)
{
	printf("%c",G->vex[v]);
	G->visited[v] = 1;    
	for(int i = 1;i<=G->vexnum;i++) {
		if(!G->visited[i] && G->arcs[v][i] == 1) {
			DFS(G, i);
		} 
	} 
}

int FirstAdj(AdjMatrix *G, int v) 
{
	for(int i = 1; i <= G->vexnum; i++) {
		if(G->arcs[v][i] == 1 && G->visited[i] != 1) {
			return i;
		}
	}
	return -1;
}

int NextAdj(AdjMatrix *G,int v,int w)
{
	for(int i = w + 1; i <= G->vexnum; i++) {
		if(G->arcs[v][i] == 1 && G->visited[i] != 1) {
			return i;
		}
	}
	return -1;
}

void BFS(AdjMatrix *G,int v) {
	printf("%c", G->vex[v]);
	G->visited[v] = 1;
	SqQueue *Q = (SqQueue *)malloc(sizeof(SqQueue));
	Q->front = 0;
	Q->rear = 0;
	Q->front++;
	Q->vex[Q->front] = v;
	while(Q->front-Q->rear != 0) {
		v = Q->vex[Q->front];
		Q->rear++;
		int w = FirstAdj(G,v);
		while(w!=-1) {
			if(!G->visited[w]) {
				printf("%c", G->vex[w]);
				G->visited[w] = 1;
				Q->front++;
				Q->vex[Q->front] = w;
			}
			w = NextAdj(G,v,w);
		}
	}
}


void JieDian(AdjMatrix *G) 
{
	int i, j;
	int in[MAX];
	int on[MAX];
	for(i = 1;i <= G->vexnum; i++) {
		int count = 0;
		for(j = 1; j <= G->vexnum; j++) {
			if(G->arcs[i][j] == 1) {
				count++;
			}
		}
		on[i] = count;
	}
	for(i = 1; i <= G->vexnum; i++) {
		int count = 0;
		for(j = 1; j <= G->vexnum; j++) {
			if(G->arcs[j][i] == 1) {
				count++;
			}
		}
		in[i] = count;
	}
	for(i = 1; i <= G->vexnum; i++) {
		printf("%c %d %d %d\n", G->vex[i], on[i], in[i], in[i]+on[i]);
	}
}

int main()
{
	AdjMatrix *G = (AdjMatrix *)malloc(sizeof(AdjMatrix));
	Create(G);
	JieDian(G);
	InitiaLise(G);
	DFS(G,1);
	printf("\n");
	InitiaLise(G);
	BFS(G,1);
	return 0;
}



