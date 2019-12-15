/*************************************************************************
	> File Name: 哈弗曼数.c
	> Author: 
	> Mail: 
	> Created Time: 2019年11月30日 星期六 20时19分13秒
 ************************************************************************/
#include<stdio.h>
#include<string.h> 
#include<stdlib.h>

#define MAX 32768
#define N 6
#define M 2*N-1
#define NUM 99
#define CH 30

typedef char* HuffmanCode[N];
typedef char charcode[CH];
typedef char numcode[NUM];

typedef struct {
    int LChild;
    int RChild; 
    int weight;
    int parent;
} HTNode, HafmanTree[M]; 

void nSelect(HafmanTree ht, int pos, int *s1, int *s2);


void CrtHafmanTree(HafmanTree ht,int w[],int n) {
    int i,s1,s2,m;
    m = 2*n-1;
    for(i = 0; i < n;i++){
        ht[i].weight = w[i];
        ht[i].parent = -1;
        ht[i].LChild = -1;
        ht[i].RChild = -1; 
    }
    for(i = n; i < m; i++){
        ht[i].weight = 0;
        ht[i].parent = -1;
        ht[i].LChild = -1;
        ht[i].RChild = -1;
    }
    for(i = n; i < m; i++) {
        nSelect(ht, i-1, &s1, &s2);
        ht[i].weight = ht[s1].weight + ht[s2].weight;
        ht[s1].parent = i;
        ht[s2].parent = i;
        ht[i].LChild = s1;
        ht[i].RChild = s2;
    }
}
void nSelect(HafmanTree ht, int pos, int *s1, int *s2){
    int i, j, m1, m2;
    m1 = m2 = MAX;
    for(j = 0; j <= pos; j++) {
        if(ht[j].weight < m1 && ht[j].parent == -1) {
            m2 = m1;
			*s2 = *s1;
            *s1 = j;
			m1 = ht[j].weight;
        }
        else if(ht[j].weight < m2 && ht[j].parent == -1){
            m2 = ht[j].weight;
            *s2 = j;
        }
    }
}
void CrtHuffmanCode(HafmanTree ht, HuffmanCode hc, int n){
    char *cd;
	int start, c, p, i;
    cd = (char*)malloc(n*sizeof(char));
    cd[n-1] = '\0';   
    for(i = 0; i < n; i++){
        start = n-1;
		c = i;
        p = ht[i].parent;
        while(p != -1){
            --start;
            if(ht[p].LChild == c) 
				cd[start] = '0';
            else    
				cd[start] = '1';
            c = p;
			p = ht[p].parent;
        }
        hc[i] = (char*)malloc((n-start)*sizeof(char));        
        strcpy(hc[i], &cd[start]);

    }
    free(cd);
}
void printcode(char s[], HuffmanCode hc){
    for(int i = 0; i < N; i++){
        printf("%c:", s[i]);
        printf("%s\n", hc[i]);

    }
}
void ncode(charcode hk, char s[], HuffmanCode hc){
    int j;
    char *p = hk;  
    while(*p != '\0'){    
        for(j = 0; j < N; j++){
            if(*p == s[j])
                printf("%s", hc[j]);
        }
        p++;
    }
    printf("\n");   
}
void  nprint(numcode hs, HafmanTree ht, char s[]){
    char *p = hs;
	int key;
	HTNode g;
    while(*p != '\0'){
        g = ht[M-1];           
        while(g.LChild != -1 && g.RChild != -1 && (*p != '\0')) {
            switch(*p){              
            case '0':
                key = g.LChild; 
                g = ht[g.LChild];
				break;
            case '1':
                key = g.RChild;
                g = ht[g.RChild];
				break;
            }
            p++;
        }
        printf("%c", s[key]);
    }
}
int main(){ 
    HafmanTree ht;
	HuffmanCode hc;
	charcode hk;
	numcode hs;
    char s[N]={'A','B','C','D','E','F'};
    int  w[N], i;
    for(i = 0; i < N; i++)
        scanf("%d", &w[i]);
    scanf("%s", hk);
    scanf("%s", hs);
    CrtHafmanTree(ht, w, 6);
    CrtHuffmanCode(ht, hc, 6);
    printcode(s, hc);
    ncode(hk, s, hc); 
    nprint(hs, ht, s);
}

