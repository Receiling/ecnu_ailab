#include <bits/stdc++.h>

using namespace std;

struct Point
{
    double x, y;
};
Point cities[155];
double cost[155][155];
int path[305][150];
int cnt;
double best;
double local_best;
double p = 0.02;
int pop_num = 200;

/**********用于计算两点之间的距离**********/
double dis(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

/**********从文件读入城市信息**********/
void read()
{
    FILE *fp_location = fopen("Cities(144).txt", "r");
    cnt = 0;
    double xx, yy;
    while(fscanf(fp_location, "%d%lf%lf", &cnt, &xx, &yy) != EOF)
    {
        cities[cnt].x = xx;
        cities[cnt].y = yy;
    }
    fclose(fp_location);
    for(int i = 1; i <= cnt; ++i)
        for(int j = 1; j <= i; ++j)
    {
        if(i == j)
        {
            cost[i][j] = 0.0;
            continue;
        }
        cost[i][j] = cost[j][i] = dis(cities[i], cities[j]);
    }
    return;
}

/**********生成一组随机解**********/
void GeneratePermutation(int pop)
{
	for(int i = 0; i < cnt; i++) path[pop][i] = i + 1;

	int high, low;
	int rand_index;

    int sqrt_n = (int)sqrt(1.0 * cnt);

	for(int i = 0; i < cnt; i++)
	{
		// 考虑到N的数值可能很大，随机数由两段拼接而成
		high = rand() % sqrt_n;
		low = rand()% sqrt_n;
		rand_index = high * sqrt_n + low;
        //if(rand_index == 0) continue;
		swap(path[pop][i], path[pop][rand_index]);
	}
	return;
}

/**************评价函数*******************/
double evaluate(int* path)
{
    double sum = 0.0;
    for(int i = 0; i < cnt; ++i)
        sum = sum + cost[path[i]][path[(i + 1) % cnt]];
    return sum;
}

/*****************交换a,b两个位置的城市*****************/
void inverse(int a, int b, int* path)
{
    int tmp = (b - a + 1) / 2;
    for(int i = 0; i < tmp; ++i)
    {
        int t = path[a + i];
        path[a + i] = path[b - i];
        path[b - i] = t;
    }
    return;
}

/************在一组解中找x所在的位置**************/
int find_x(int x, int* path)
{
    for(int i = 0; i < cnt; ++i)
        if(path[i] == x) return i;
}

/*****************局部搜索*****************/
void Local_Search()
{
    best = 10000000000.0;

    for(int i = 0; i < pop_num; ++i)
        GeneratePermutation(i);
    int cot = 0;
    while(cot < 800)
    {
        local_best = best;
        for(int i = 0; i < pop_num; ++i)
        {
            int tmp_path[155] = {0};
            for(int j = 0;j < cnt; ++j)
                tmp_path[j] =  path[i][j];
            int cur = rand() % cnt + 1;
            int st = 0;
            int ed = 0;
            int next = 0;
            while(true)
            {
                st = find_x(cur, tmp_path);
                double r = (rand() * 1.0) / (1.0 * RAND_MAX);
                if(r > p)
                {
                    int line = rand() % pop_num;
                    int tmp = find_x(cur, path[line]);
                    tmp = (tmp + 1) % cnt;
                    next = path[line][tmp];
                }
                else
                {
                    next = rand() % cnt + 1;
                }
                ed = find_x(next, tmp_path);
                if(abs(st - ed) <= 1) break;
                inverse(min(st, ed) + 1, max(st, ed), tmp_path);
                cur = next;
            }
            int e1 = evaluate(tmp_path);
            int e2 = evaluate(path[i]);
            if(e1 < e2)
            {
                for(int j = 0; j < cnt; ++j)
                    path[i][j] = tmp_path[j];
                if(e1 < local_best)
                    local_best = e1;
            }
        }
        if(local_best < best)
        {
            best = local_best;
            cot = 0;
        }
        else cot += 1;
    }
    return;
}

int main()
{
    read();
    double min_x = 100000000000.0;
    //for(int i = 0; i < 50; ++i)
    {
        srand((unsigned)time(0));
        Local_Search();
        if(best < min_x)
            min_x = best;
        //printf("%.3f\n", best);
    }
    printf("%.3f\n", min_x);
    return 0;
}

