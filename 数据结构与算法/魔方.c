/*************************************************************************
	> File Name: 魔方.c
	> Author: 
	> Mail: 
	> Created Time: 2019年10月30日 星期三 18时55分35秒
 ************************************************************************/

#include<stdio.h>


void Init(int m, int M[50][50])
{
    for(int i = 0; i < m; i++) 
    {
        for(int j = 0; j < m; j++) {
            M[i][j] = 0;
        }
    }
}

void deal(int m, int M[50][50])
{
    int i;
    int x = 0;
    int y = m/2;
    M[x][y] = 1;
    for(i = 2; i <= (m*m); i++) {
        x = (x - 1 + m) % m;
        y = (y + 1 + m) % m;
        if(M[x][y] != 0) {
            x = (x + 2) % m;
            y = (y - 1 + m) % m;
        }
        M[x][y] = i;
    }

}

void Print(int m, int M[50][50])
{
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            printf("%03d ", M[i][j]);
        }
        printf("\n");
    }
}

void main(int argc, char *argv[])
{
    int m;
    int M[50][50];
    scanf("%d", &m);
    Init(m, M);
    deal(m, M);
    Print(m, M);
}
