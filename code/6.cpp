#pragma warning( disable : 4996)
#include <stdio.h>
#include  <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
using namespace std;
#define maxn 99999   //��󶥵����
#define INF 99999999	//Ȩֵ����
int parent[maxn];
bool visited[maxn];
typedef struct info {
	char* stationName;
	char* rodeName;
}info;

typedef struct arcnode {
	int index;
	int val;
	struct arcnode* next;
}AR;

typedef struct MyGraph {
	int arcnum, vexnum;
	info *vexname;
	AR* N;
}GH;

struct node
{
	int id;
	int w;
	friend bool operator < (node a, node b)
	{
		return a.w > b.w;
	}
};
node d[maxn];
int findvex(char* s,char *t, GH* G)
{
	int i;
	for (i = 0; i < G->vexnum; i++) {
		if (!strcmp(s, G->vexname[i].stationName) && !strcmp(t,G->vexname[i].rodeName))
			return i;
	}
	return -1;
}

int findStation(char* s, GH* G)
{
	int i;
	for (int i = 0; i < G->vexnum; i++) {
		if (!strcmp(s, G->vexname[i].stationName))
			return i;
	}
	return -1;
}

int side[maxn];

void Insert(GH* G, int a, int b)
{
	AR* q = (AR*)malloc(sizeof(AR));
	if (q == NULL) {
		printf("error\n");
		exit(0);
	}
	q->index = b;
	q->val = 1;
	q->next = G->N[a].next;
	G->N[a].next = q;
}

void ShowGraph(GH* G)
{
	
	for (int i = 0; i < G->vexnum; i++) {
		printf("%s :", G->vexname[i].stationName);
		AR* p = G->N[i].next;
		while (p) {
			printf("%s %s->",G->vexname[p->index].rodeName, G->vexname[p->index].stationName);
			p = p->next;
		}
		printf("\n");
	}
}

void CreateGH1(GH* G)
{
	int i, j = 0, pos,num = 0;
	FILE* fp;
	char roadName[40], str[50], buf[3000];
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	memset(side, 0, sizeof(side));
	fp = fopen("data.txt", "r");
	if (fp == NULL)
		printf("open file erro\n");
	//��ʼ��ͼ
	G->arcnum = 0;
	G->vexnum = 0;
	G->vexname = (info*)malloc(maxn * sizeof(info));
	G->N = (AR*)malloc(maxn * sizeof(AR));
	//���ļ��ж�����
	while (!feof(fp)) {
		fscanf(fp, "%s", roadName);		//��ȡ�Ǽ�·��
		fgets(buf, sizeof(buf), fp);		//��ȡ��һ·�����е�վ��
		for (i = 0; buf[i]; i++) {
			if (buf[i] != ',' && buf[i] != ' ' && buf[i] != '\n')
			{
				str[j] = buf[i];
				j++;
			}
			else if (buf[i] == ',' || buf[i] == '\n') {
				pos = findStation(str, G);
				if (pos == -1) {  //����ó�վ��ǰ��û�У���ô��ͼ�м��������վ
					G->vexname[G->vexnum].stationName = (char*)malloc((strlen(str) + 1) * sizeof(char));
					G->vexname[G->vexnum].rodeName = (char*)malloc((strlen(roadName) + 1) * sizeof(char));
					strcpy(G->vexname[G->vexnum].stationName, str);
					strcpy(G->vexname[G->vexnum].rodeName, roadName);
					G->N[G->vexnum].next = NULL;
					side[num] = G->vexnum;
					G->vexnum++;
				}
				else {				//��������վ�У��ͱ�������λ����side��
					side[num] = pos;
				}
				memset(str, 0, sizeof(str));
				j = 0;
				num++;		//վ������+1
			}
		}
		for (i = 0; i < num-1; i++) {
			for (int k = i+1; k < num; k++) {
				Insert(G, side[i],side[k]);
				Insert(G, side[k], side[i]);
			}
		}
		num = 0;
		memset(side, 0, sizeof(side));		//���side
	}
}

void CreateGH2(GH* G)
{
	int i, j = 0, pos, num = 0,pre = -1;
	FILE* fp;
	char roadName[40], str[50], buf[3000];
	memset(str, 0, sizeof(str));
	memset(buf, 0, sizeof(buf));
	memset(side, 0, sizeof(side));
	fp = fopen("data.txt", "r");
	if (fp == NULL)
		printf("open file erro\n");
	//��ʼ��ͼ
	G->arcnum = 0;
	G->vexnum = 0;
	G->vexname = (info*)malloc(maxn * sizeof(info));
	G->N = (AR*)malloc(maxn * sizeof(AR));
	//���ļ��ж�����
	while (!feof(fp)) {
		fscanf(fp, "%s", roadName);		//��ȡ�Ǽ�·��
		fgets(buf, sizeof(buf), fp);		//��ȡ��һ·�����е�վ��
		for (i = 0; buf[i]; i++) {
			if (buf[i] != ',' && buf[i] != ' ' && buf[i] != '\n')
			{
				str[j] = buf[i];
				j++;
			}
			else if (buf[i] == ',' || buf[i] == '\n') {
				pos = findStation(str, G);
				if (pos == -1) {  //����ó�վ��ǰ��û�У���ô��ͼ�м��������վ
					G->vexname[G->vexnum].stationName = (char*)malloc((strlen(str) + 1) * sizeof(char));
					G->vexname[G->vexnum].rodeName = (char*)malloc((strlen(roadName) + 1) * sizeof(char));
					strcpy(G->vexname[G->vexnum].stationName, str);
					strcpy(G->vexname[G->vexnum].rodeName, roadName);
					G->N[G->vexnum].next = NULL;
					if (pre == -1)
						pre = G->vexnum;
					else {
						Insert(G, pre, G->vexnum);
						Insert(G, G->vexnum, pre);
						pre = G->vexnum;
					}
					G->vexnum++;
				}
				else {
					if (pre == -1)
						pre = pos;
					else {
						Insert(G, pre, pos);
						Insert(G, pos, pre);
						pre = pos;
					}
				}
				
				memset(str, 0, sizeof(str));
				j = 0;
				num++;		//վ������+1
			}
		}
		num = 0;
		pre = -1;
		memset(side, 0, sizeof(side));		//���side
	}
}

priority_queue<node> q;
void Dijkstra(GH * G,int s)
{
	for (int i = 0; i < G->vexnum; i++) {
		//��ʼ��
		d[i].id = i;
		d[i].w = INF;		
		parent[i] = -1;		
		visited[i] = false;		
	}
	d[s].w = 0;			//Դ�㵽Դ�����·����0
	q.push(d[s]);		//ѹ�������
	while (!q.empty()) {
		node cd = q.top();
		q.pop();
		int u = cd.id;
		if (visited[u])
			continue;
		visited[u] = true;
		AR* p = G->N[u].next;
		//�ɳڲ���
		while (p != NULL) {
			//�������������ڵĶ��㣬�����ɳڲ��������¹�����룬ѹ�����
			int v = p->index;
			if (!visited[v] && d[v].w > d[u].w + p->val) {
				d[v].w = d[u].w + p->val;
				parent[v] = u;
				q.push(d[v]);
			}
			p = p->next;
		}
	}
}

int main()
{
	char start[30],end[30],choice_str[100];
	int ch, st, ed;
	GH G1, G2;
	CreateGH1(&G1);
	CreateGH2(&G2);
	while (1) {
		memset(d, 0, sizeof(d));
		printf("\n================================������·ϵͳ================================\n");
		printf("��������Ҫ��ѯ��ѡ� 1.ת������·�ߡ�2.����վ������·�ߡ�\n");
		scanf("%s", choice_str);
		ch = atoi(choice_str);
		printf("�����������յ㣺(����վ�������ÿո����)\n");
		scanf("%s %s", start, end);
		if (ch == 1) {
			st = findStation(start, &G1);
			ed = findStation(end, &G1);
			if (st == -1 || ed == -1) {
				printf("�����ڸ�վ");
				continue;
			}
			Dijkstra(&G1, st);
			stack<int> s;
			if (d[ed].w != INF) {
				printf("����ת�ߴ���Ϊ %d��\n", d[ed].w - 1);
				for (int i = parent[ed]; i != -1; i = parent[i]) {
					s.push(i);
				}
				printf("·��Ϊ��");
				while (!s.empty()) {
					int temp = s.top();
					s.pop();
					printf("%s--->", G1.vexname[temp]);
				}
				printf("%s\n", G1.vexname[ed]);
			}
			else
				printf("�����������·");
		}
		else if (ch == 2) {
			st = findStation(start, &G2);
			ed = findStation(end, &G2);
			if (st == -1 || ed == -1) {
				printf("�����ڸ�վ");
				continue;
			}
			Dijkstra(&G2, st);
			stack<int> s;
			if (d[ed].w != INF) {
				printf("���پ���վ����Ϊ %d��\n", d[ed].w + 1);
				for (int i = parent[ed]; i != -1; i = parent[i]) {
					s.push(i);
				}
				printf("·��Ϊ��");
				while (!s.empty()) {
					int temp = s.top();
					s.pop();
					printf("%s--->", G1.vexname[temp]);
				}
				printf("%s\n", G1.vexname[ed]);
			}
			else
				printf("�����������·");
		}

	}
	return 0;
}
