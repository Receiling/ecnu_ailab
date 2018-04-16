#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;

vector<int> edge[1005];
int pre[1005];
struct Point
{
    double x, y;
};
struct Node
{
    int id;
    double cost;
    friend bool operator < (Node a, Node b)
    {
        return a.cost > b.cost;
    }
};
const double INF = 1000000000000000.0;
const double eps = 0.00000000001;
Point cities[1005];
double cost[1005][1005];
bool used[1005];
int cnt;
double dis[1005];

double distance(Point p1, Point p2)//用于计算两点之间的距离
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

void read()//从文件读入城市信息
{
    FILE *fp_location = fopen("Cities(100000).txt", "r");
    FILE *fp_link = fopen("Cities(100000)link.txt", "r");
    cnt = 0;
    double xx, yy;
    while(fscanf(fp_location, "%d%lf%lf", &cnt, &xx, &yy) != EOF)
    {
        cities[cnt].x = xx;
        cities[cnt].y = yy;
    }
    fclose(fp_location);
    for(int i = 0; i <= cnt; ++i)
        for(int j = 0; j <= cnt; ++j)
        cost[i][j] = INF;
    for(int i = 0; i <= cnt; ++i)
        cost[i][i] = 0.0;
    int x, y;
    while(fscanf(fp_link, "%d%d", &x, &y) != EOF)
    {
        //printf("%d %d\n", x, y);
        if(x != y)
        {
            edge[x].push_back(y);
            cost[x][y] = distance(cities[x], cities[y]);
        }
    }
    fclose(fp_link);
    return;
}

bool Dijkstra(int st, int ed)//Dijkstra算法
{
    memset(pre, -1, sizeof(pre));
    memset(used, 0, sizeof(used));
    //如果和起点相连，着dis的值为和起点之间的距离，否则，dis为无穷大
    for(int i = 0; i <= 1000; ++i)
        dis[i] = INF;
    dis[st] = 0.0;
    //pre数组用于记录每一个节点的前一个节点
    for(int i = 0; i <= cnt; ++i)
    {
        if(dis[i] > cost[st][i])
        {
            dis[i] = cost[st][i];
            pre[i] = st;
        }
    }
    pre[st] = -1;
    used[st] = true;
    while(true)
    {
        int v = -1;
        for(int u = 0; u <= cnt; ++u)//找一个dis数组中的最小值
        {
            if(!used[u] && (v == -1 || (dis[u] < dis[v]) ) ) v = u;
        }
        if(v == -1) break;
        used[v] = true;
        for(int u = 0; u <= cnt; ++u)//更新dis数组，如果起点到j经过v的距离更短，那就更新起点到j的距离
        {
            if(dis[u] > dis[v] + cost[v][u])
            {
                dis[u] = dis[v] + cost[v][u];
                pre[u] = v;
            }
        }
    }
    if(INF - dis[ed] < eps)
        return false;
    return true;
}


bool A(int st, int ed)
{
    memset(used, 0, sizeof(used));
    memset(pre, -1, sizeof(pre));
    for(int i = 0; i <= cnt; ++i)
    {
        dis[i] = INF;
    }
    double tmp_dis[1005];
    for(int i = 0; i <= cnt; ++i)
        tmp_dis[i] = distance(cities[ed], cities[i]);
    priority_queue<Node> open;//采用优先队列(重载operator <,使得优先队列的顶部元素始终f值最小）作为open表
    open.push((Node){st, tmp_dis[st]});
    dis[st] = 0.0;
    while(!open.empty())
    {
        Node node = open.top();
        open.pop();
        int id = node.id;
        //printf("%3d   %.8f  %.8f  %.8f\n", id, node.cost, dis[id], tmp_dis[id]);
        if(id == ed) return true;
        /*{
            printf("%.8f\n", dis[ed]);
        }*/
        for(int i = 0; i < edge[id].size(); ++i)
        {
            int tmp = edge[id][i];
            if(used[tmp])//扩展出的节点在close表中
            {
                if(dis[tmp] > dis[id] + cost[id][tmp])
                {
                    dis[tmp] = dis[id] + cost[id][tmp];
                    pre[tmp] = id;
                    used[tmp] = false;
                    open.push((Node){tmp, dis[tmp] + tmp_dis[tmp]});
                }
            }
            else
            {
                if(INF - dis[tmp] < eps)//扩展出的是新节点
                {
                    dis[tmp] = dis[id] + cost[id][tmp];
                    pre[tmp] = id;
                    open.push((Node){tmp, dis[tmp] + tmp_dis[tmp]});
                }
                else//扩展出的节点在open表中，与已经在open表中的节点比较g(n),如果经过新的节点距离更小
                //那么直接复制一个open表中的节点，然后更新新f(n)，并把这个节点作为一个新节点插入优先队列中
                {
                    if(dis[tmp] > dis[id] + cost[id][tmp])
                    {
                        dis[tmp] = dis[id] + cost[id][tmp];
                        pre[tmp] = id;
                        open.push((Node){tmp, dis[tmp] + tmp_dis[tmp]});
                    }
                }
            }
        }
        used[id] = true;
    }
    return false;
}

void print(int ed)//打印路径
{
    stack<int> path;
    int tmp = ed;
    while(pre[tmp] != -1)
    {
        tmp = pre[tmp];
        path.push(tmp);
    }
    while(!path.empty())
    {
        printf(" %d ->", path.top());
        path.pop();
    }
    printf(" %d\n", ed);
    return;
}

int main()
{
    read();
    int st, ed;
    printf("请输入起始城市和目标城市:\n");
    while(scanf("%d%d", &st, &ed) != EOF)
    {

        if(Dijkstra(st, ed))
        {
            printf("长度:%.8f\n", dis[ed]);
            printf("路径：");
            print(ed);
        }
        else
            printf("不存在从%d到%d的路径\n", st, ed);
        printf("\n\n");
        if(A(st, ed))
        {
            printf("长度:%.8f\n", dis[ed]);
            printf("路径：");
            print(ed);
        }
        else
            printf("不存在从%d到%d的路径\n", st, ed);
        printf("\n\n");
        printf("\n\n请输入起始城市和目标城市:\n");
    }
    return 0;
}
