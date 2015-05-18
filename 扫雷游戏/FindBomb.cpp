#include"FindBomb.h"
void OpenMap(int x, int y)
{
	if (MapState[x][y] == 1 || MapState[x][y] == 2)
		return;
	MapState[x][y] = 1;
	if (MapContest[x][y] == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			int fx = x + direct_x[i];
			int fy = y + direct_y[i];
			if (fx < 0 || fy < 0 || fx >= cxMap || fy >= cyMap)
				continue;
			OpenMap(fx, fy);
		}
	}
}
bool IsWin()
{
	for (int i = 0; i < cxMap; i++)
	{
		for (int j = 0; j < cyMap; j++)
		{
			if (MapState[i][j] == 1)
				continue;
			if (MapState[i][j] == 0)
				return false;
			if (MapContest[i][j] != -1)
				return false;
		}
	}
	return true;
}
BOOL GameInitialize(HINSTANCE hInstance)
{
	g_pGE = new GameEngine(hInstance, TEXT("魔塔"), TEXT("魔塔"),
		IDI_BIGICON, IDI_SMALLICON, FALSE, 32, 900, 500);
	if (g_pGE == NULL)
		return false;
	g_pGE->SetFrameDelay(33);
	g_pGE->SetPause(false);

	if (!g_pGE->CreateGameWindow())	//创建游戏窗口
	{
		MessageBox(NULL, TEXT("修改显示模式失败!\r\n程序退出!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;		//退出程序
	}
	return true;
}

void GameStart(HWND hWnd)
{
	memset(MapContest, 0, sizeof(MapContest));
	memset(MapState, 0, sizeof(MapState));
	int x, y;
	srand((unsigned)time(NULL));
	for (int i = 0; i < BombNum; i++)
	{
		do
		{
			x = rand() % cxMap;
			y = rand() % cyMap;
		} while (MapContest[cxMap][cyMap] == -1);
		MapContest[x][y] = -1;
		for (int j = 0; j < 8; j++)
		{
			int fx = x + direct_x[j];
			int fy = y + direct_y[j];
			if (fx < 0 || fx >= cxMap || fy < 0 || fy >= cyMap)
				continue;
			if (MapContest[fx][fy] != -1)
			{
				MapContest[fx][fy]++;
			}
		}
	}
}

void GameAction(HWND hWnd)
{
	if (IsWin())
	{
		if (MessageBox(g_pGE->GetWindow(), TEXT(" 恭喜你胜利了，要继续吗"), TEXT("退出游戏"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES)
			GameStart(hWnd);
		else
			DestroyWindow(hWnd);
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

void LButtonDown(HWND hWnd, int xPos, int yPos, WPARAM wParam)
{
	int x = xPos / cxBlock;
	int y = yPos / cyBlock;
	if (MapState[x][y] != 0)
		return;
	switch (MapContest[x][y])
	{
	case -1:
		if (MessageBox(g_pGE->GetWindow(), TEXT(" 你挂了，要重新开始吗？"), TEXT("退出游戏"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES)
			GameStart(hWnd);
		else
			DestroyWindow(hWnd);
		break;
	default:
		OpenMap(x, y);
		break;
	}
}
void RButtonDown(HWND hWnd, int xPos, int yPos, WPARAM wParam)
{
	int x = xPos / cxBlock;
	int y = yPos / cyBlock;
	switch (MapState[x][y])
	{
	case 0:
		MapState[x][y] = 2;
		break;
	case 1:
		break;
	case 2:
		MapState[x][y] = 0;
		break;
	}
}

void KeyEvent(HWND hwnd, WPARAM wParam){}
void CharEvent(HWND hwnd, WPARAM wParam){}

void GamePaint(HDC hdc)
{
	RECT rect;
	HBRUSH hbClose = CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH hbOpen = CreateSolidBrush(RGB(200, 200, 200));
	HBRUSH hbFlag = CreateSolidBrush(RGB(255, 0, 0));
	HFONT hf = CreateFont(15, 10, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, 0, 0, 0, 0, 0);
	HFONT hf_old = (HFONT)SelectObject(hdc, hf);

	for (int y = 0; y < cyMap; y++)
	{
		for (int x = 0; x < cxMap; x++)
		{
			SetRect(&rect, x*cxBlock + 3, y*cyBlock + 3, (x + 1)*cxBlock - 3, (y + 1)*cyBlock - 3);
			switch (MapState[x][y])
			{
			case 0://close
				FillRect(hdc, &rect, hbClose);
				break;
			case 1://open
				switch (MapContest[x][y])
				{
				case 0:
					FillRect(hdc, &rect, hbOpen);
					break;
				default:
					FillRect(hdc, &rect, hbOpen);
					TCHAR Buffer[20];
					int size;
					size = wsprintf(Buffer, TEXT("%d"), MapContest[x][y]);
					TextOut(hdc, x*cxBlock + 10, y*cyBlock + 7, Buffer, size);
					break;
				}
				break;
			case 2://flag
				FillRect(hdc, &rect, hbFlag);
				break;
			}
		}
	}

	SelectObject(hdc, hf_old);
	DeleteObject(hf);
	DeleteObject(hbClose);
	DeleteObject(hbFlag);
	DeleteObject(hbOpen);
}

void GameEnd()
{
	delete g_pGE;
};
void GameActive(HWND hWnd)
{
	g_pGE->SetPause(FALSE);
};
void GamePause(HWND hWnd)
{
	g_pGE->SetPause(TRUE);
};
BOOL GameWindowClose(HWND hWnd)
{
	if (MessageBox(g_pGE->GetWindow(), TEXT("你确定要退出吗？"), TEXT("退出游戏"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK) == IDYES)
		return TRUE;
	else
		return FALSE;
};