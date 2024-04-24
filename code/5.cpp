#include <iostream>
#include <algorithm>
using namespace std;

#define MAXN 200000

struct E
{
    int start;//边的顶点
    int end;//边的终点
    int weight;//边的权重
}Edge[MAXN+10];

bool cmp(E a,E b)
{
    return a.weight<b.weight;
}

int father[100010];

inline int find(int x)//并查集
{
    while(father[x]!=x)
    {
        x = father[x];
    }
    return x;
}

inline void unite(int x,int y)
{
    father[find(x)] = find(y);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL),cout.tie(NULL);
    int n,m;
    cin>>n>>m; //输入点的数量和边的数量
    int start,end,weight;
    for(int i=1;i<=m;++i)
    {
        cin>>start>>end>>weight;
        Edge[i].start = start;
        Edge[i].end = end;
        Edge[i].weight = weight;
    }
    sort(Edge+1,Edge+m+1,cmp);
    for(int i=1;i<=n;++i)
    {
        father[i] = i;
    }
    for(int i=1;i<=m;++i)
    {
        if(find(Edge[i].start) != find(Edge[i].end))
        {
            unite(find(Edge[i].start),find(Edge[i].end));
        }
        if(find(1) == find(n))
        {
            cout<<Edge[i].weight;
            break;
        }
    }
    return 0;
}
