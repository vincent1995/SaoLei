#include"GameEngine.h"
#include"resource.h"
#include<time.h>
#include<stdlib.h>
//��������ָ��
GameEngine* g_pGE;
//�����ͼ��Ϣ
const int BombNum = 10;
int flagNum;
const int cxMap=15;
const int cyMap = 15;
const int cxBlock = 30;
const int cyBlock = 30;
//��ͼ���ݱ�־
/*
-1 for  bomb
0 for blank
1~8 for tips
*/
int  MapContest[cxMap][cyMap];
//��ͼ״̬��־
/*
0 for close
1 for open 
2 for flaged
*/
int MapState[cxMap][cyMap];

int direct_x[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int direct_y[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
