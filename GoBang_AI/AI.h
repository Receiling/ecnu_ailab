#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

#define GRID_NUM 15
#define BLACK 0
#define WHITE 1
#define NONE -1
#define STWO         1  //�߶�
#define STHREE       2  //����
#define SFOUR        3  //����
#define TWO          4  //���
#define THREE        5  //����
#define FOUR         6  //����
#define FIVE         7  //����

int moves[4][4] = {-1, -1, 1, 1,    -1, 1, 1, -1,    0, 1, 0, -1,    -1, 0, 1, 0};
//����/����  ����/����  ��/��  ��/��
int dir[4][2] = {1, 1,  1, -1,  0, 1,  1, 0};

int PosValue[GRID_NUM][GRID_NUM]=
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
{0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
{0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
{0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
{0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
{0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
{0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
{0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
{0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
{0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
{0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
{0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

class AI
{
public:
    struct point
    {
        int x, y;
        int value;
        bool operator < (const point& ano) const
        {
            return value > ano.value;
        }
    };
private:
    int model[2][10];
    bool used[GRID_NUM][GRID_NUM][4];
    point steps[230];
    int siz;
    int turn;//turn=0,��ʾ���֣�ִ�ڣ�turn = 1, ��ʾ���֣�ִ��
    int next_x, next_y;
    int dep;
public:
    int chessboard[GRID_NUM][GRID_NUM];
    AI();
    ~AI();

    void setPos(int x, int y, int color);
    int getPos(int x, int y);
    void setDep(int x);
    void getNextStep();
    int getX();
    int getY();
    void setTurn(int _turn);
    bool isGameOver(int x, int y);
    void coverwin();
    void print();
    void getModel(int x, int y, int z);
    int calculate(int color);
    int evaluate(int color);
    bool isValid(int x, int y);
    int GenerateSteps(int color);
    int MINMAX_Search(int min_max, int alpha, int beta, int depth, int color);
};
