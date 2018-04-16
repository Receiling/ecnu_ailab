#include <windows.h>
#include <commdlg.h>
#include <tchar.h>
#include "resource.h"
#include "AI.h"


//构造函数和析构函数
AI::AI()
{
    siz = 15;
    turn = 1;
    next_x = 7;
    next_y = 7;
    dep = 2;
    memset(chessboard, -1, sizeof(chessboard));
   /* memset(wins, 0, sizeof(wins));
   //统计所有可能的赢法,需要好好理解
    int cnt = 0;
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 11; j++){
            for (int k = 0; k < 5; k++){
                wins[i][j+k][cnt] = true;
            }
            cnt++;
        }
    }
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 15; j++){
            for (int k = 0; k < 5; k++){
                wins[i+k][j][cnt] = true;
            }
            cnt++;
        }
    }
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++){
            for (int k = 0; k < 5; k++){
                wins[i+k][j+k][cnt] = true;
            }
            cnt++;
        }
    }
    for (int i = 0; i < 11; i++){
        for (int j = 14; j > 3; j--){
            for (int k = 0; k < 5; k++){
                wins[i+k][j-k][cnt] = true;
            }
            cnt++;
        }
    }
    //共 572 赢法*/
}
AI::~AI()
{

}
/**********************************************************************************************/
//判断合法性
bool AI::isValid(int x, int y)
{
    return x >= 0 && x < siz && y >= 0 && y < siz;
}
/**********************************************************************************************/
//在棋盘上落子等一系列操作
void AI::setPos(int x, int y, int color)
{
    chessboard[x][y] = color;
}
int AI::getPos(int x, int y)
{
     return chessboard[x][y];
}
void AI::setDep(int x)
{
    dep = x;
    return;
}
bool AI::isGameOver(int x, int y)
{
    memset(model, 0, sizeof(model));
    memset(used, 0, sizeof(used));

    for(int z = 0; z < 4; ++z)
    {
       getModel(x, y, z);
    }
    if(model[chessboard[x][y]][FIVE] >= 1) return true;
    return false;
}
void AI::getNextStep()
{
    int tmp = MINMAX_Search(1, -10000, 10000, dep, turn);
    chessboard[next_x][next_y] = turn;
    return;
}
int AI::getX()
{
    return next_x;
}
int AI::getY()
{
    return next_y;
}
void AI::setTurn(int _turn)
{
   turn = _turn;
    return;
}
void AI::print()
{
    for(int i = 0; i < 15; ++i)
        for(int j = 0; j < 15; ++j)
        printf("%d%c", chessboard[i][j] == -1 ? 8 : chessboard[i][j], j == 14 ? '\n' : ' ');
    return;
}
/**********************************************************************************************/

//评价机制，合理的进行攻守的转换
//model[color][i]代表颜色为color的棋i棋型有几个
void AI::getModel(int x, int y, int z)
{
    int dx = dir[z][0];//表示分析棋型的z方向，总共有4各方向
    int dy = dir[z][1];
    int color = chessboard[x][y];
    used[x][y][z] = true;
    //水平方向检测,竖直方向检测,右斜上方检测左斜上方检测
    int xx = 0, yy = 0;
    int cnt = 1;//记录最多能放的棋子数
    int left_block = 0, right_block = 0;//记录气的个数
    xx = x;
    yy = y;
    while(isValid(xx - dx, yy - dy))
    {
        xx -= dx;
        yy -= dy;
        if(chessboard[xx][yy] != color) break;
        used[xx][yy][z] = true;
        cnt += 1;
    }
    if(chessboard[xx][yy] != color)
    {
        xx += dx;
        yy += dy;
    }
    while(isValid(xx - dx, yy - dy))
    {
        xx -= dx;
        yy -= dy;
        if(chessboard[xx][yy] != NONE) break;
        left_block += 1;
    }
    xx = x;
    yy = y;
    while(isValid(xx + dx, yy + dy))
    {
        xx += dx;
        yy += dy;
        if(chessboard[xx][yy] != color) break;
        used[xx][yy][z] = true;
        cnt += 1;
    }
    if(chessboard[xx][yy] != color)
    {
        xx -= dx;
        yy -= dy;
    }
    while(isValid(xx + dx, yy + dy))
    {
        xx += dx;
        yy += dy;
        if(chessboard[xx][yy] != NONE) break;
        right_block += 1;
    }
    if(cnt >= 5)
    {
        model[color][FIVE] += 1;
    }
    else if(cnt == 4)
    {
        if(left_block >= 1 && right_block >= 1)
            model[color][FOUR] += 1;
        else if(left_block >= 1 || right_block >= 1)
            model[color][SFOUR] += 1;
    }
    else if(cnt == 3)
    {
        if(left_block >= 1 && right_block >= 1)
            model[color][THREE] += 1;
        else if(left_block >= 2 || right_block >= 2)
            model[color][STHREE] += 1;
    }
    else if(cnt == 2)
    {
        if( (left_block >= 1 && right_block >= 2) || (left_block >= 2 && right_block >= 1))
            model[color][TWO] += 1;
        else if(left_block >= 3 || right_block >= 3)
            model[color][STWO] += 1;
    }
    return;
}

int AI::calculate(int color)
{
	if (color == WHITE) {
		if (model[BLACK][FIVE])
			return -9999;
		if (model[WHITE][FIVE])
			return 9999;
	}
	else {
		if (model[BLACK][FIVE])
			return 9999;
		if (model[WHITE][FIVE])
			return -9999;
	}

	//两个冲四等于一个活四
	if (model[WHITE][SFOUR]>1)
		model[WHITE][FOUR]++;
	if (model[BLACK][SFOUR]>1)
		model[BLACK][FOUR]++;

	int wValue = 0, bValue = 0;

	//轮到白棋走
	if (color == WHITE) {
		if (model[WHITE][FOUR])
			return 9990;//活四,白胜返回极值

		if (model[WHITE][SFOUR])
			return 9980;//冲四,白胜返回极值

		if (model[BLACK][SFOUR])
			return 9960;//黑棋有死四而白棋无四，一定要堵

		if (model[BLACK][FOUR])
			return -9970;//白无冲四活四,而黑有活四,黑胜返回极值

		if (model[BLACK][SFOUR] && model[BLACK][THREE])
			return -9960;//而黑有冲四和活三,黑胜返回极值

		if (model[WHITE][THREE] && model[BLACK][SFOUR] == 0)
			return 9950;//白有活三而黑没有四,白胜返回极值

		if (model[BLACK][THREE] > 1 && model[WHITE][SFOUR] == 0 && model[WHITE][THREE] == 0 && model[WHITE][STHREE] == 0)
			return -9940;//黑的活三多于一个,而白无四和三,黑胜返回极值

		if (model[WHITE][THREE] > 1)
			wValue += 2000;//白活三多于一个,白棋价值加2000
		else if (model[WHITE][THREE])//否则白棋价值加200
			wValue += 200;

		if (model[BLACK][THREE] > 1)
			bValue += 500;//黑的活三多于一个,黑棋价值加500
		else if (model[BLACK][THREE])//否则黑棋价值加100
			bValue += 100;

		//每个眠三加10
		if (model[WHITE][STHREE])
			wValue += (model[WHITE][STHREE] * 10);
		//每个眠三加5
		if (model[BLACK][STHREE])
			bValue += (model[BLACK][STHREE] * 5);

		//每个活二加4
		if (model[WHITE][TWO])
			wValue += (model[WHITE][TWO] * 4);
		//每个活二加2
		if (model[BLACK][STWO])
			bValue += (model[BLACK][TWO] * 2);

		//每个眠二加1
		if (model[WHITE][STWO])
			wValue += model[WHITE][STWO];
		//每个眠二加1
		if (model[BLACK][STWO])
			bValue += model[BLACK][STWO];
	}
	//轮到黑棋走
	else {
		if (model[BLACK][FOUR])
			return 9990;//活四,黑胜返回极值

		if (model[BLACK][SFOUR])
			return 9980;//冲四,黑胜返回极值

        if (model[WHITE][SFOUR])
			return 9960;//白棋有死四而黑棋无四，一定要堵

		if (model[WHITE][FOUR])
			return -9970;//活四,白胜返回极值

		if (model[WHITE][SFOUR] && model[WHITE][THREE])
			return -9960;//冲四并活三,白胜返回极值

		if (model[BLACK][THREE] && model[WHITE][SFOUR] == 0)
			return 9950;//黑活三,白无四。黑胜返回极值

		if (model[WHITE][THREE] > 1 && model[BLACK][SFOUR] == 0 && model[BLACK][THREE] == 0 && model[BLACK][STHREE] == 0)
			return -9940;//白的活三多于一个,而黑无四和三,白胜返回极值

		if (model[BLACK][THREE] > 1)//黑的活三多于一个,黑棋价值加2000
			bValue += 2000;
		else if (model[BLACK][THREE])//否则黑棋价值加200
			bValue += 200;

		if (model[WHITE][THREE] > 1)//白的活三多于一个,白棋价值加500
			wValue += 500;
		else if (model[WHITE][THREE])//否则白棋价值加100
			wValue += 100;

		//每个眠三加10
		if (model[WHITE][STHREE])
			wValue += (model[WHITE][STHREE] * 10);
		//每个眠三加5
		if (model[BLACK][STHREE])
			bValue += (model[BLACK][STHREE] * 5);

		//每个活二加4
		if (model[WHITE][TWO])
			wValue += (model[WHITE][TWO] * 4);
		//每个活二加2
		if (model[BLACK][STWO])
			bValue += (model[BLACK][TWO] * 2);

		//每个眠二加1
		if (model[WHITE][STWO])
			wValue += model[WHITE][STWO];
		//每个眠二加1
		if (model[BLACK][STWO])
			bValue += model[BLACK][STWO];
	}

	//加上所有棋子的位置价值
	for (int i = 0; i < siz; ++i)
		for (int j = 0; j < siz; ++j) {
			if (chessboard[i][i] == BLACK)
				bValue += PosValue[i][j];
			else
				wValue += PosValue[i][j];
		}

	//返回估值
	if (color == BLACK)
		return bValue - wValue;
	else
		return wValue - bValue;
}

int AI::evaluate(int color)
{

    memset(model, 0, sizeof(model));
    memset(used, 0, sizeof(used));

    for(int i = 0; i < siz; ++i)
        for(int j = 0; j < siz; ++j)
        {
            if(chessboard[i][j] != NONE)
            {
                for(int z = 0; z < 4; ++z)
                {
                    if(!used[i][j][z])
                        getModel(i, j, z);
                }
            }
        }
    return calculate(color);
}
/**********************************************************************************************/

//生成合法着法
int AI::GenerateSteps(int color)
{
    int cnt = 0;
    for(int i = 0; i < siz; ++i)
        for(int j = 0; j < siz; ++j)
    {
        if(chessboard[i][j] == NONE)
        {
            chessboard[i][j] = color;
            steps[cnt].x = i;
            steps[cnt].y = j;
            steps[cnt++].value = evaluate(color);
            chessboard[i][j] = NONE;
        }
    }
    sort(steps, steps + cnt);
    return cnt;
}
/**********************************************************************************************/
//博弈树的搜索极大极小化搜索
//极大极小搜索，min_max代表是极大节点还是极小节点，alpha为下界，beta为上界， depth为深度，
//min_max = 1表示极大节点，min_max = 0表示极小节点
int AI::MINMAX_Search(int min_max, int alpha, int beta, int depth, int color)
{
    if(depth == 0) return evaluate(color);
    int n = GenerateSteps(color);
    for(int i = 0; i < n; ++i)
    {
        point p = steps[i];
        chessboard[p.x][p.y] = color;
        int tmp = -MINMAX_Search(1 - min_max, -beta, -alpha, depth - 1, 1 - color);
        chessboard[p.x][p.y] = NONE;
        if(tmp >= beta)
            return beta;
        if(tmp > alpha)
        {
            alpha = tmp;
            next_x = p.x;
            next_y = p.y;
        }
    }
    return alpha;
}
/**********************************************************************************************/





//以下为窗口程序

LRESULT CALLBACK WndProc      (HWND, UINT, WPARAM, LPARAM) ;
HINSTANCE Hinstance;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("GoBang_AI") ;
     MSG          msg ;
     HWND         hwnd ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1) ;
     wndclass.lpszClassName = TEXT ("GoBang_AI") ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Failed to Registerclass!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     Hinstance = hInstance;
     hwnd = CreateWindow (szAppName, TEXT ("GoBang_AI"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          800, 660,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
     }
     return msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static HBITMAP hBitmap, white_hbitmap, black_hbitmap, select_hbitmap;
     static BITMAP bitmap, white_bitmap, black_bitmap, select_bitmap;
     static HINSTANCE hInstance ;
     PAINTSTRUCT ps;
     HDC hdc, hdcMem ;
     HMENU hMenu = ::GetMenu( hwnd );
     int wmId    = LOWORD(wParam);
     int wmEvent = HIWORD(wParam);
     TCHAR str[ 100 ];
     static AI ai;
     static DWORD Difficult, Turn;
     int cxSource, cySource;
     static int cx, cy, px, py;
     static int color;
     static HDC select_hdc;

     switch (message)
     {
     case WM_CREATE :
          hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;
          hBitmap = LoadBitmap (hInstance, MAKEINTRESOURCE(IDB_BITMAP1)) ;
          GetObject (hBitmap, sizeof (BITMAP), &bitmap) ;
          white_hbitmap = LoadBitmap (hInstance, MAKEINTRESOURCE(IDB_BITMAP2)) ;
          GetObject (white_hbitmap, sizeof (BITMAP), &white_bitmap) ;
          black_hbitmap = LoadBitmap (hInstance, MAKEINTRESOURCE(IDB_BITMAP3)) ;
          GetObject (black_hbitmap, sizeof (BITMAP), &black_bitmap) ;
          select_hbitmap = LoadBitmap (hInstance, MAKEINTRESOURCE(IDB_BITMAP4)) ;
          GetObject (select_hbitmap, sizeof (BITMAP), &select_bitmap) ;
          CheckMenuRadioItem( hMenu, ID_32774, ID_32774, ID_32774, MF_BYCOMMAND );
          Difficult = ID_32774;
          CheckMenuRadioItem( hMenu, ID_32778, ID_32778, ID_32778, MF_BYCOMMAND );
          Turn = ID_32778;
          select_hdc = GetDC (hwnd);
          return 0 ;

     case WM_COMMAND :
          switch (wmId)
          {
            case ID_32771 :
                MessageBoxA(hwnd, TEXT("人工智能作业\nAI 五子棋\n学号:   10152130137\n姓名:   汪贻俊\n"), TEXT("About AI_GoBang"), MB_ICONINFORMATION);
                break ;
            case ID_32772 :
                if(Difficult == ID_32774)
                {
                    ai.setDep(2);
                }
                else if(Difficult == ID_32775)
                {
                    ai.setDep(3);
                }
                else
                {
                    ai.setDep(1);
                }
                if(Turn == ID_32777)
                {
                    ai.setTurn(0);
                    color = 1;
                    ai.chessboard[7][7] = 0;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                else
                {
                    ai.setTurn(1);
                    color = 0;
                }
                break;
            case ID_32773 :
                memset(ai.chessboard, -1, sizeof(ai.chessboard));
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            case ID_32780 :
                if(MessageBoxA(hwnd, TEXT("你确定要退出吗？\n"), TEXT("提示对话框"), MB_ICONQUESTION | MB_YESNO) == IDYES)
                {
                    DestroyWindow(hwnd);
                }
                break;
            case ID_32777 :
            case ID_32778 :
                if(wmId != Turn)
                {
                    CheckMenuRadioItem(hMenu, ID_32777, ID_32778, wmId, MF_BYCOMMAND);
                    Turn = wmId;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                break;
                break;
            case ID_32774 :
            case ID_32775 :
            case ID_32776 :
                if(wmId != Difficult)
                {
                    CheckMenuRadioItem(hMenu, ID_32774, ID_32776, wmId, MF_BYCOMMAND);
                    Difficult = wmId;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                break;
          }
          return 0 ;
     case WM_LBUTTONDOWN:
          px = cx ;
          py = cy ;
          InvalidateRect(hwnd, NULL, TRUE);
          return 0 ;

     case WM_MOUSEMOVE:
         {
              int tmpx = LOWORD (lParam) ;
              int tmpy = HIWORD (lParam) ;
              cx = (tmpx - 18) / 40;
              cy = (tmpy - 14) / 40;
              if(tmpx < cx * 40 + 18 - 20) cx -= 1;
              if(tmpx > cx * 40 + 18 + 20) cx += 1;
              if(tmpy < cy * 40 + 14 - 20) cy -= 1;
              if(tmpy > cy * 40 + 14 + 20) cy += 1;
              //SendMessage(hwnd, WM_PAINT, 0, 0);
         }
        return 0 ;

     case WM_LBUTTONUP:
        if(px >= 0 && px < 15 && py >= 0 && py < 15 && ai.getPos(py, px) == NONE)
        {
            hdc = GetDC (hwnd) ;
            ai.setPos(py, px, color);
            hdcMem = CreateCompatibleDC (hdc) ;
            for(int i = 0; i < 15; ++i)
                for(int j = 0; j < 15; ++j)
            {
                if(ai.chessboard[i][j] == WHITE)
                {
                    SelectObject (hdcMem, white_hbitmap) ;
                    cxSource = white_bitmap.bmWidth ;
                    cySource = white_bitmap.bmHeight ;
                    BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                }
                else if(ai.chessboard[i][j] == BLACK)
                {
                    SelectObject (hdcMem, black_hbitmap) ;
                    cxSource = black_bitmap.bmWidth ;
                    cySource = black_bitmap.bmHeight ;
                    BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                }
            }
            if(ai.isGameOver(py, px))
            {
                MessageBoxA(hwnd, TEXT("恭喜你！！！\n你打败了五子棋AI!!!\n"), TEXT("About AI_GoBang"), MB_ICONINFORMATION);
                memset(ai.chessboard, -1, sizeof(ai.chessboard));
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else
            {
                ai.getNextStep();
                for(int i = 0; i < 15; ++i)
                    for(int j = 0; j < 15; ++j)
                {
                    if(ai.chessboard[i][j] == WHITE)
                    {
                        SelectObject (hdcMem, white_hbitmap) ;
                        cxSource = white_bitmap.bmWidth ;
                        cySource = white_bitmap.bmHeight ;
                        BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                    }
                    else if(ai.chessboard[i][j] == BLACK)
                    {
                        SelectObject (hdcMem, black_hbitmap) ;
                        cxSource = black_bitmap.bmWidth ;
                        cySource = black_bitmap.bmHeight ;
                        BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                    }
                }
                if(ai.isGameOver(ai.getX(), ai.getY()))
                {
                    MessageBoxA(hwnd, TEXT("对不起！！！\n你输给了五子棋AI!!!\n"), TEXT("About AI_GoBang"), MB_ICONINFORMATION);
                    memset(ai.chessboard, -1, sizeof(ai.chessboard));
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            DeleteDC (hdcMem) ;
            ReleaseDC (hwnd, hdc) ;
        }
        return 0 ;
     case WM_PAINT :
    {
         hdc = BeginPaint(hwnd, &ps);
         TCHAR text[100] = {0};
         int siz = wsprintf(text, TEXT("%d   %d"), px, py);
         TextOut(hdc, 700, 100, text, siz);
         hdcMem = CreateCompatibleDC (hdc) ;
         SelectObject (hdcMem, hBitmap) ;
         cxSource = bitmap.bmWidth ;
         cySource = bitmap.bmHeight ;
         BitBlt (hdc, 0, 0, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
         for(int i = 0; i < 15; ++i)
                for(int j = 0; j < 15; ++j)
            {
                if(ai.chessboard[i][j] == WHITE)
                {
                    SelectObject (hdcMem, white_hbitmap) ;
                    cxSource = white_bitmap.bmWidth ;
                    cySource = white_bitmap.bmHeight ;
                    BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                }
                else if(ai.chessboard[i][j] == BLACK)
                {
                    SelectObject (hdcMem, black_hbitmap) ;
                    cxSource = black_bitmap.bmWidth ;
                    cySource = black_bitmap.bmHeight ;
                    BitBlt (hdc, j * 40, i * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                }
            }
         /*if(cx >= 0 && cx < 15 && cy >= 0 && cy < 15 && ai.getPos(cy, cx) == NONE)
         {
                SelectObject (hdcMem, select_hbitmap) ;
                cxSource = select_bitmap.bmWidth ;
                cySource = select_bitmap.bmHeight ;
                BitBlt (hdc, cx * 40, cy * 40, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
         }*/
         DeleteDC (hdcMem) ;
         EndPaint(hwnd, &ps);
         return 0;
    }
     case WM_DESTROY :
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}

