#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream> 
using namespace std;
#define MAX 999 //���������ô��վ�� 
#define INF 9999999  //��ʼ������룬����Ϊ����� 
struct Node
{
    long long length;//��¼���ĳ���
    int type;//��¼��������
};
struct Node Graph[MAX][MAX]; //�ڽӾ��󴢴� 
//sum_long��sum_short,�ֱ𱣴�ÿ����㵽������Ĵ��·��֮�͡�С��·��֮��
long long sum_short[MAX];
long long sum_short_square[MAX];
long long sum_long[MAX];
long long d[MAX]; 
int pre[MAX]; //pre����������·����ÿ������ǰ�����
int visited[MAX]; //�����Ѿ����ʵĵط� 
int ans=INF;
long long square(long long x)
{
    return x*x;
}
void Dijkstra(int s,int n)
{
    memset(visited,0,sizeof(visited));//��ʼ������
    fill(sum_long,sum_long+MAX,INF);
    fill(sum_short,sum_short+MAX,INF);
    fill(sum_short_square,sum_short_square+MAX,INF);
    fill(pre,pre+MAX,0);
    fill(d,d+MAX,INF);
    d[s]=0;
    sum_long[s]=0;
    sum_short[s]=0;
    while(true)
    {
        int u=-1;
        long long mini=INF;//�ߵ���ǰ�ڵ���Сֵ 
        for(int i=1;i<=n;i++)
        {
            if(visited[i]==0&&d[i]<mini) //d[i]�ߵ���i ���ܸ�ֵ ����break �൱�ڸ�����mini,�ҵ���һ����u 
            {
                u=i;//������ ���û�߹�����uΪ��1��2��3��4��5��6�� 
                mini=d[i];//��¼��ǰ����С�� ��0��9��25��45��66��76�� 
            }
        }
        if(u==-1) break;
        visited[u]=1; //�˵��߹� 
        cout<<u<<"->";
        for(int v=1;v<=n;v++) //����uѰ��v 
        {
            if(Graph[u][v].length!=INF&&visited[v]==0) //��· ��������һ����û�߹� 
            {
                if(Graph[u][v].type==1)//С· 
                {
                    long long d1=INF;
                    long long d2=INF;
                    if(sum_short[u]!=INF)//�����С����Դ�������,��ǰ����С· 
                    {
                        //��Ҫ����һ��������������Ǵ����С��������ֵ����
                        if(pre[u]==0) d1=square(sum_short[u]+Graph[u][v].length);
                        else d1=sum_long[pre[u]]+square(sum_short[u]+Graph[u][v].length);//���ܳ���aͨ�������b������bͨ��С����c����
                                                                               //cͨ��С����d���������
                    }
                    if(sum_long[u]!=INF)//���ǰ���Ǵ�· 
                        d2=square(Graph[u][v].length)+sum_long[u];
                    if(d1<d2)
                    {
                        sum_short[v]=min(sum_short[v],sum_short[u]+Graph[u][v].length);//���½���Ӧ��С����Ȩ
                        sum_short_square[v]=min(d1,sum_short_square[v]);
                    }
                    else
                    {
                        sum_short[v]=min(sum_short[v],Graph[u][v].length);//������һ����·�Ǵ��������������Ҫ��ͷ��ʼ����С���ĵ�Ȩ
                        sum_short_square[v]=min(d2,sum_short_square[v]);
                        pre[v]=u;//��¼��ǰ����ǰ�����
                    }
                    d[v]=min(sum_short_square[v],sum_long[v]);
                }
                else//��ǰ��·�Ǵ��
                {
  
                    long long d1=INF;
                    long long d2=INF;
                    if(sum_long[u]!=INF)  d1=sum_long[u]+Graph[u][v].length;
                    if(sum_short[u]!=INF) d2=sum_short_square[u]+Graph[u][v].length;
                    long long mini=min(d1,d2);
                    sum_long[v]=min(sum_long[v],mini);
                    d[v]=min(sum_short_square[v],sum_long[v]);
                }
            }
        }
    }
    return;
}
 
int main()
{
    int n,m;
    int t,a,b;
    long long c;
    ifstream infile("input.txt");
    //scanf("%d %d",&n,&m);
    infile>>n>>m;
    for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=n;j++)
        {
            Graph[i][j].length=INF;
            Graph[j][i].length=INF;
        }
    }
    for(int i=0;i<m;i++)
    {
        infile>>t>>a>>b>>c;
        //scanf("%d%d%d%lld",&t,&a,&b,&c);
        if(t==1)//С��
        {
            Graph[a][b].length=c;
            Graph[b][a].length=c;
            Graph[a][b].type=1;
            Graph[b][a].type=1;
        }
        else
        {
            Graph[a][b].length=c;
            Graph[b][a].length=c;
            Graph[a][b].type=0;
            Graph[b][a].type=0;
        }
    }
    infile.close(); 
    cout<<"�����յ�·��Ϊ��\n";
    Dijkstra(1,n);
    cout<<"\n"<<endl;
    for(int i=1;i<=n;i++)
    cout<<"�����"<<i<<"��·�ڵ���Сƣ��ֵΪ��"<<d[i]<<endl; 
    cout<<endl;
    printf("�������Ϊ�� %lld\n",d[n]);
    cout<<endl;
    system("pause");
    return 0;
}
/*
6 7
1 1 2 3
1 2 3 2
0 1 3 30
0 3 4 20
0 4 5 30
1 3 5 6
1 5 6 1
* 
*/
