#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream> 
using namespace std;
#define MAX 999 //假设最多这么多站点 
#define INF 9999999  //初始最初距离，表现为无穷大 
struct Node
{
    long long length;//记录道的长度
    int type;//记录道的类型
};
struct Node Graph[MAX][MAX]; //邻接矩阵储存 
//sum_long、sum_short,分别保存每个结点到出发点的大道路径之和、小道路径之和
long long sum_short[MAX];
long long sum_short_square[MAX];
long long sum_long[MAX];
long long d[MAX]; 
int pre[MAX]; //pre数组来保存路径上每个结点的前驱结点
int visited[MAX]; //储存已经访问的地方 
int ans=INF;
long long square(long long x)
{
    return x*x;
}
void Dijkstra(int s,int n)
{
    memset(visited,0,sizeof(visited));//初始化数据
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
        long long mini=INF;//走到当前节点最小值 
        for(int i=1;i<=n;i++)
        {
            if(visited[i]==0&&d[i]<mini) //d[i]走到了i 就能赋值 否者break 相当于更新了mini,找到下一个点u 
            {
                u=i;//遍历点 如果没走过就是u为（1，2，3，4，5，6） 
                mini=d[i];//记录当前的最小数 （0，9，25，45，66，76） 
            }
        }
        if(u==-1) break;
        visited[u]=1; //此点走过 
        cout<<u<<"->";
        for(int v=1;v<=n;v++) //到达u寻找v 
        {
            if(Graph[u][v].length!=INF&&visited[v]==0) //有路 并且另外一个点没走过 
            {
                if(Graph[u][v].type==1)//小路 
                {
                    long long d1=INF;
                    long long d2=INF;
                    if(sum_short[u]!=INF)//如果有小道与源结点相连,即前面有小路 
                    {
                        //需要处理一种特殊情况，就是大道、小道交错出现的情况
                        if(pre[u]==0) d1=square(sum_short[u]+Graph[u][v].length);
                        else d1=sum_long[pre[u]]+square(sum_short[u]+Graph[u][v].length);//可能出现a通过大道与b相连，b通过小道与c相连
                                                                               //c通过小道与d相连的情况
                    }
                    if(sum_long[u]!=INF)//如果前面是大路 
                        d2=square(Graph[u][v].length)+sum_long[u];
                    if(d1<d2)
                    {
                        sum_short[v]=min(sum_short[v],sum_short[u]+Graph[u][v].length);//更新结点对应的小道点权
                        sum_short_square[v]=min(d1,sum_short_square[v]);
                    }
                    else
                    {
                        sum_short[v]=min(sum_short[v],Graph[u][v].length);//由于上一条道路是大道，所以这里需要从头开始考虑小道的点权
                        sum_short_square[v]=min(d2,sum_short_square[v]);
                        pre[v]=u;//记录当前结点的前驱结点
                    }
                    d[v]=min(sum_short_square[v],sum_long[v]);
                }
                else//当前道路是大道
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
        if(t==1)//小道
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
    cout<<"到达终点路线为：\n";
    Dijkstra(1,n);
    cout<<"\n"<<endl;
    for(int i=1;i<=n;i++)
    cout<<"到达第"<<i<<"个路口的最小疲劳值为："<<d[i]<<endl; 
    cout<<endl;
    printf("到达最后为： %lld\n",d[n]);
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
