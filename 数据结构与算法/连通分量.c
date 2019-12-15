/*************************************************************************
	> File Name: 连通分量.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月12日 星期四 20时31分11秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#define MAX 20

int visited[MAX] = { 0 };

typedef struct {
	int arcnum;
	int vernum;
	int arcs[MAX][MAX];
	char vertex[MAX];
} AdjMatrix;

void creat(AdjMatrix *G)
{
    int i, j, k;
	scanf("%d %d", &G->vernum, &G->arcnum);
	getchar();
	for (i = 1; i <= G->vernum; i++) {
		scanf("%c", &G->vertex[i]);
	}
	getchar();
	for (i = 1; i <= G->vernum; i++)
	{
		for (j = 1; j <= G->vernum; j++)
		{
			G->arcs[i][j] = 0;
		}
	}
	for (i = 0; i < G->arcnum; i++)
	{
		int head = 0, tail = 0; 
		char chead, ctail; 
		scanf("%c%c", &chead, &ctail);
		getchar(); 
		for (j = 1; j <= G->vernum; j++) {
			if (G->vertex[j] == chead) {
				head = j;
				for (k = 1; k <= G->vernum; k++) {
					if (G->vertex[k] == ctail)
						tail = k;
				}
			}
		}
		G->arcs[head][tail] = 1;
		G->arcs[tail][head] = 1;
	}
}

void DFS(AdjMatrix *G, int start)
{
    int i;
	visited[start] = 1;
	for (i = 1; i <= G->vernum; i++) {
        if (G->arcs[start][i] == 1 && visited[i] == 0) {
            DFS(G, i);
        }
	}
}

int DFSTraverse(AdjMatrix *G)
{
	int count = 0, i;
    for (i = 1; i <= G->vernum; i++) {
		visited[i] = 0;
    }
	for (i = 1; i <= G->vernum; i++) {
		if (!visited[i]) {
			DFS(G, i);
			count++;
		}
	}
	return count;
}

int main() 
{
	int answer;
	AdjMatrix *G;
	G = (AdjMatrix*)malloc(sizeof(AdjMatrix));
	creat(G);
	answer = DFSTraverse(G);
	printf("%d",answer);
	return 0;
}

