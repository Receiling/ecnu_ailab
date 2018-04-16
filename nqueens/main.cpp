#include <iostream>
#include <cstring>
#include <cstdio>
#include <queue>
#include <cmath>
#include <time.h>
#include <stack>

using namespace std;

#define BACKDATA(i) if(!used[0][i] && !used[1][i + cur] && !used[2][i - cur + N]){graph[cur][i] = used[0][i] = used[1][i + cur] = used[2][i - cur + N] = 1;backdata(cur + 1);if(had) return;graph[cur][i] = used[0][i] = used[1][i + cur] = used[2][i - cur + N] = 0;}

int graph[105][105];
int ans[105];
int used[3][105];

int N;
bool had;//���ڱ���Ƿ��Ѿ��ҵ���
struct Point//����һ�������еĽڵ�
{
    int ans[31];
    int cnt;
    Point()
    {
        cnt = 0;
    }
};

void backdata1(int cur)
{
    if(had) return;
    if(cur == N)
    {
        had = 1;
        return;
    }
    for(int i = 0; i < N; ++i)
    {
        if(!used[0][i] && !used[1][i + cur] && !used[2][i - cur + N])
        {
            graph[cur][i] = used[0][i] = used[1][i + cur] = used[2][i - cur + N] = 1;
            backdata1(cur + 1);
            if(had) return;
            graph[cur][i] = used[0][i] = used[1][i + cur] = used[2][i - cur + N] = 0;
        }
    }
    return;
}

void backdata(int cur)//�ݹ���ݵķ���
{
    if(had) return;
    if(cur == N)
    {
        had = 1;
        return;
    }
    int len = N / 2;
    if(2 * cur + 2 < N)//����������е�ֵ��2��С��N����ô��ȡ���оʹ��м�������ȡ����Ϊ�м���еĶԺ�����е�Ӱ���С
    {
        if(N & 1)
        {
            for(int i = 0; i <= len; ++i)
            {
                if(i == 0)
                {
                    BACKDATA(len);
                }
                else
                {
                    BACKDATA(len - i);
                    BACKDATA(len + i);
                }
            }
        }
        else
        {
            for(int i = 0; i < len; ++i)
            {
                BACKDATA(len - i - 1);
                BACKDATA(len + i);
            }
        }
    }
    else//���򣬾ʹ��������м�ȡ����Ϊ��ʱ�Ժ����е�Ӱ���൱���������ߵ���Ӧ��Ӱ���λ�ý���
    {
        for(int i = 0; i < len; ++i)
        {
            BACKDATA(i);
            BACKDATA(N - 1 - i);
        }
    }
    return;
}

void bfs()
{
    queue<Point> que;//��һ����������open�е�Ԫ�أ���֤���������
    for(int i = 0; i < N; i++)
    {
        Point point;
        point.ans[point.cnt] = i;
        point.cnt += 1;
        que.push(point);
    }
    while(!que.empty())
    {
        Point point = que.front();
        que.pop();
        int cnt = point.cnt;
        int temp[105];
        for(int i = 0; i < cnt; ++i)
        {
            temp[i] = point.ans[i];
        }
        if(cnt == N)
        {
            for(int i = 0; i < cnt; ++i)
            {
                graph[i][temp[i]] = 1;
            }
            break;
        }
        for(int i = 0; i < N; ++i)
        {
            bool ok = 1;
            for(int j = 0; j < cnt; ++j)
            {
                if(i == temp[j] || abs(i - temp[j]) == abs(cnt - j))//�жϳ�ͻ
                {
                    ok = 0;
                    break;
                }
            }
            if(ok)
            {
                Point p = point;
                p.ans[cnt] = i;
                p.cnt += 1;
                que.push(p);
            }
        }
    }
    return;
}

void dfs1(int cur)
{
    if(had) return;
    if(cur == N)
    {
        had = 1;
        return;
    }
    for(int i = 0; i < N; ++i)
    {
        bool ok = 1;
        for(int j = 0; j < cur; ++j)
        {
            if(ans[j] == i || abs(ans[j] - i) == abs(j - cur))
            {
                ok = 0;
                break;
            }
        }
        if(ok)
        {
            ans[cur] = i;
            graph[cur][i] = 1;
            dfs1(cur + 1);
            if(had) return;
            graph[cur][i] = 0;
        }
    }
    return;
}

void dfs()
{
    stack<Point> que;//��һ��ջ����open�е�Ԫ�أ���֤�������
    for(int i = 0; i < N; i++)
    {
        Point point;
        point.ans[point.cnt] = i;
        point.cnt += 1;
        que.push(point);
    }
    while(!que.empty())
    {
        Point point = que.top();
        que.pop();
        int cnt = point.cnt;
        int temp[105];
        for(int i = 0; i < cnt; ++i)
        {
            temp[i] = point.ans[i];
        }
        if(cnt == N)
        {
            for(int i = 0; i < cnt; ++i)
            {
                 graph[i][temp[i]] = 1;
            }
            break;
        }
        for(int i = 0; i < N; ++i)
        {
            bool ok = 1;
            for(int j = 0; j < cnt; ++j)
            {
                if(i == temp[j] || abs(i - temp[j]) == abs(cnt - j))
                {
                    ok = 0;
                    break;
                }
            }
            if(ok)
            {
                Point p = point;
                p.ans[cnt] = i;
                p.cnt += 1;
                que.push(p);
            }
        }
    }
    return;

}
int main()
{
    //freopen("ans.txt", "w", stdout);
    while(scanf("%d", &N) != EOF)
    {
        had = 0;
        memset(graph, 0, sizeof(graph));
        memset(used, 0, sizeof(used));
        clock_t startTime = clock();//��¼ʱ��
        backdata(0);
        //bfs();
        //dfs();
        //�����������N���ʺ�������
        /*for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < N; ++j)
                printf("%d%c", graph[i][j], j == N - 1 ? '\n' : ' ');
        }*/
        //�������ÿһ�еĻʺ��λ��
        printf("( ");
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < N; ++j)
            {
                if(graph[i][j])
                {
                    printf("%d%c", j + 1, i == N - 1 ? ' ' : ',');
                }
            }
        }
        printf(")\n");
        clock_t endTime = clock();
        if(had)
        {
            printf("N = %d,    Time =  %.6fs\n", N, (double)(endTime - startTime) / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
