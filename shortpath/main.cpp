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

double distance(Point p1, Point p2)//���ڼ�������֮��ľ���
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

void read()//���ļ����������Ϣ
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

bool Dijkstra(int st, int ed)//Dijkstra�㷨
{
    memset(pre, -1, sizeof(pre));
    memset(used, 0, sizeof(used));
    //����������������dis��ֵΪ�����֮��ľ��룬����disΪ�����
    for(int i = 0; i <= 1000; ++i)
        dis[i] = INF;
    dis[st] = 0.0;
    //pre�������ڼ�¼ÿһ���ڵ��ǰһ���ڵ�
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
        for(int u = 0; u <= cnt; ++u)//��һ��dis�����е���Сֵ
        {
            if(!used[u] && (v == -1 || (dis[u] < dis[v]) ) ) v = u;
        }
        if(v == -1) break;
        used[v] = true;
        for(int u = 0; u <= cnt; ++u)//����dis���飬�����㵽j����v�ľ�����̣��Ǿ͸�����㵽j�ľ���
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
    priority_queue<Node> open;//�������ȶ���(����operator <,ʹ�����ȶ��еĶ���Ԫ��ʼ��fֵ��С����Ϊopen��
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
            if(used[tmp])//��չ���Ľڵ���close����
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
                if(INF - dis[tmp] < eps)//��չ�������½ڵ�
                {
                    dis[tmp] = dis[id] + cost[id][tmp];
                    pre[tmp] = id;
                    open.push((Node){tmp, dis[tmp] + tmp_dis[tmp]});
                }
                else//��չ���Ľڵ���open���У����Ѿ���open���еĽڵ�Ƚ�g(n),��������µĽڵ�����С
                //��ôֱ�Ӹ���һ��open���еĽڵ㣬Ȼ�������f(n)����������ڵ���Ϊһ���½ڵ�������ȶ�����
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

void print(int ed)//��ӡ·��
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
    printf("��������ʼ���к�Ŀ�����:\n");
    while(scanf("%d%d", &st, &ed) != EOF)
    {

        if(Dijkstra(st, ed))
        {
            printf("����:%.8f\n", dis[ed]);
            printf("·����");
            print(ed);
        }
        else
            printf("�����ڴ�%d��%d��·��\n", st, ed);
        printf("\n\n");
        if(A(st, ed))
        {
            printf("����:%.8f\n", dis[ed]);
            printf("·����");
            print(ed);
        }
        else
            printf("�����ڴ�%d��%d��·��\n", st, ed);
        printf("\n\n");
        printf("\n\n��������ʼ���к�Ŀ�����:\n");
    }
    return 0;
}
