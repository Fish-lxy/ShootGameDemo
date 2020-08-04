#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include<graphics.h>
#include<Windows.h>
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<iostream>
#include<list>

#include "Ammo.h"
#include "Enemy.h"

void initGameWindow(COLORREF backgroundcolor);//初始化
void mainLoop();//主循环
void DrawScore();//绘制分数
void DrawCannon(POINT* Point);//绘制大炮
double GetPointsDistance(int x1, int y1, int x2, int y2);//求两点距离
void CreateAmmo();//子弹创建
void MoveAmmo();//子弹移动
void DeleteAmmo();//子弹销毁
void CreateEnemy();//敌人创建
void RandEnemyDataCreate();//随机生成敌人初始数据
void MoveEnemy();//敌人移动
void EnemyTouch();//敌人碰撞检测
void MouseLeftDownEvent();//鼠标左键按下检测
void DEBUG();//DEBUG窗口



HWND MainWindow;
HWND ForegroundWindow;
POINT mousePoint;
std::list<Ammo*> SavedAmmo;
std::list<Ammo*>::iterator AmmoIterBegin;
std::list<Ammo*>::iterator AmmoIterEnd;
std::list<Enemy*> SavedEnemy;
std::list<Enemy*>::iterator EnemyIterBegin;
std::list<Enemy*>::iterator EnemyIterEnd;

double CannonRangleCos = 0;//大炮当前的cos值
double CannonRangleSin = 0;//大炮当前的sin值
bool MouseLeftDown;//鼠标左键是否按下
const int CannonLength = 100;//大炮的长度(externed)
const int AmmoSpeed = 50;//子弹的速度
int AmmoR = 4;//子弹大小
int Score = 0;//分数
wchar_t ScoreStr[50];

//敌人随机生成数据
int RandEnemySpeed = 0;//敌人速度
int RandEnemyX = 0;//敌人出生坐标x
int RandEnemyR = 0;//敌人半径
int RandEnemyTime = 0;//敌人生成间隔
int RandEnemyColorR = 0, RandEnemyColorG = 0, RandEnemyColorB = 0;//敌人颜色RGB数据
COLORREF RandEnemyColorRGB;

//统计数据
unsigned int FrameCount = 0;//总帧数
double FrameTimeStart = 0;//帧开始时间
double FrameTimeEnd = 0;//帧结束时间
double FrameTimeStart_Temp, FrameTimeEnd_Temp;//帧开始结束时间缓冲
double FPS = 0;//帧率
double FpsTime = 0;//帧时间

int AllAmmoCount = 0;//总生成的子弹数
int AllEnemyCount = 0;//总生成的敌人数


int main() {
	//线程准备
	std::thread DEBUG_thread(DEBUG);//debug线程
	std::thread CREATE_AMMO_thread(CreateAmmo);//子弹创建线程
	std::thread DELETE_AMMO_thread(DeleteAmmo);//子弹销毁线程
	std::thread CREATE_ENEMY_thread(CreateEnemy);//敌人创建线程
	std::thread ENEMY_TOUCH(EnemyTouch);//敌人碰撞检测线程

	DEBUG_thread.detach();
	CREATE_AMMO_thread.detach();
	DELETE_AMMO_thread.detach();
	CREATE_ENEMY_thread.detach();
	ENEMY_TOUCH.detach();


	//初始化
	initGameWindow(RGB(146, 158, 176));

	setlinecolor(RGB(124, 49, 52));
	setfillcolor(RGB(0, 0, 0));
	setlinestyle(PS_SOLID, 10);

	BeginBatchDraw();
	while (1) {
		FrameTimeStart = GetTickCount();//帧率计算
		
		mainLoop();

		FlushBatchDraw();
		Sleep(15);
		cleardevice();

		//帧率计算
		++FrameCount;
		FrameTimeEnd = GetTickCount();
		FrameTimeStart_Temp = FrameTimeStart;
		FrameTimeEnd_Temp = FrameTimeEnd;
		
	}
	closegraph();
	return 0;
}
void initGameWindow(COLORREF backgroundcolor) {
	initgraph(800, 600, EW_SHOWCONSOLE);
	setbkcolor(backgroundcolor);
	cleardevice();
	MainWindow = FindWindow(L"EasyXWnd", L"ShootGameDemo");
	SetWindowPos(MainWindow, HWND_TOP, 600, 250, 0, 0, SWP_NOSIZE);
	system("title DEBUG");
}
void mainLoop() {
	GetCursorPos(&mousePoint);
	ScreenToClient(MainWindow, &mousePoint);

	circle(400, 600, 50);
	DrawCannon(&mousePoint);

	MoveAmmo();
	MoveEnemy();
	DrawScore();

	MouseLeftDownEvent();
}
void DrawScore() {
	static RECT scorePos = { 0, 0, 10, 10 };
	//static LOGFONT fontstyle = {};
	swprintf(ScoreStr, 50 ,L"SCORE:%d", Score);
	drawtext(ScoreStr,&scorePos, DT_SINGLELINE | DT_NOCLIP);
}
void DrawCannon(POINT* Point) {
	double adjacentside = (Point->x) - 400L;//邻边
	double hypotenuse = GetPointsDistance((Point->x), (Point->y), 400, 600);//斜边
	double oppositeside = 600L - (Point->y);//对边
	CannonRangleCos = adjacentside / hypotenuse;
	CannonRangleSin = oppositeside / hypotenuse;
	int circleX = (int)(400 + CannonLength * CannonRangleCos);
	int circleY = (int)(600 + CannonLength * -CannonRangleSin);
	line(400, 600, circleX, circleY);
}
inline double GetPointsDistance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
void CreateAmmo() {
	while (1) {
		if (MouseLeftDown == TRUE) {
			Ammo* newammo = new Ammo(AmmoSpeed, AmmoR);
			SavedAmmo.emplace_back(newammo);
			++AllAmmoCount;
			while (SavedAmmo.size() > 0 || MouseLeftDown == TRUE)
				Sleep(3);
		}
		else Sleep(3);
	}
}
void MoveAmmo() {
	AmmoIterBegin = SavedAmmo.begin();
	AmmoIterEnd = SavedAmmo.end();
	while (AmmoIterBegin != AmmoIterEnd) {
		(*AmmoIterBegin)->move();
		++AmmoIterBegin;	
	}
	AmmoIterBegin = SavedAmmo.begin();
	AmmoIterEnd = SavedAmmo.end();
}
void DeleteAmmo() {
	static std::list<Ammo*>::iterator ammoiterbeg = SavedAmmo.begin();
	static std::list<Ammo*>::iterator ammoiterend = SavedAmmo.end();
	while (1) {
		ammoiterbeg = SavedAmmo.begin();
		ammoiterend = SavedAmmo.end();
			if (MouseLeftDown == FALSE) {
				while (ammoiterbeg != ammoiterend) {
						if ((*ammoiterbeg)->getExist() == FALSE) {
							delete *ammoiterbeg;
							*ammoiterbeg = NULL;
							ammoiterbeg = SavedAmmo.erase(ammoiterbeg);
							Sleep(1);
							//break;
						}
				}
			}
		Sleep(5);
		ammoiterbeg = SavedAmmo.begin();
		ammoiterend = SavedAmmo.end();
	}
}
void CreateEnemy() {
	while (1) {
		RandEnemyDataCreate();
		Enemy* newenemy = new Enemy(RandEnemySpeed, RandEnemyX, RandEnemyR, RandEnemyColorRGB);
		SavedEnemy.emplace_back(newenemy);
		++AllEnemyCount;
		Sleep(RandEnemyTime);
	}
}
void RandEnemyDataCreate() {
	srand((int)(time(NULL) + FpsTime));
	RandEnemySpeed = (rand() % 2) + 1;
	srand((int)(time(NULL)) + FpsTime);
	RandEnemyX = (rand() % 781) + 10;
	srand((int)(time(NULL)) + FpsTime);
	RandEnemyR = (rand() % 21) + 10;
	srand((int)(time(NULL)) + FpsTime);
	RandEnemyTime = (rand() % 2001) + 1000;
	srand((int)(clock()) + FpsTime);
	RandEnemyColorR = (rand() % 256);
	srand((int)(time(NULL)) + FpsTime);
	RandEnemyColorG = (rand() % 256);
	srand((int)(clock()) - FpsTime);
	RandEnemyColorB = (rand() % 256);
	RandEnemyColorRGB = RGB(RandEnemyColorR, RandEnemyColorG, RandEnemyColorB);
}
void MoveEnemy() {
	EnemyIterBegin = SavedEnemy.begin();
	EnemyIterEnd = SavedEnemy.end();
	while (EnemyIterBegin != EnemyIterEnd) {
		(*EnemyIterBegin)->move();
		++EnemyIterBegin;
	}
	EnemyIterBegin = SavedEnemy.begin();
	EnemyIterEnd = SavedEnemy.end();
}
void EnemyTouch() {
	static std::list<Ammo*>::iterator ammoiterbeg = SavedAmmo.begin();
	static std::list<Ammo*>::iterator ammoiterend = SavedAmmo.end();
	static std::list<Enemy*>::iterator enemyiterbeg = SavedEnemy.begin();
	static std::list<Enemy*>::iterator enemyiterend = SavedEnemy.end();
	while (1) {
		enemyiterbeg = SavedEnemy.begin();
		enemyiterend = SavedEnemy.end();
		while (enemyiterbeg != enemyiterend) {
			if (*enemyiterbeg != NULL) {
				ammoiterbeg = SavedAmmo.begin();
				ammoiterend = SavedAmmo.end();
				while (ammoiterbeg != ammoiterend) {
					if (*ammoiterbeg != NULL || *enemyiterbeg != NULL) {
						if (GetPointsDistance((*ammoiterbeg)->getX(), (*ammoiterbeg)->getY(), (*enemyiterbeg)->getX(), (*enemyiterbeg)->getY()) <= (*ammoiterbeg)->getR() + (*enemyiterbeg)->getR()) {
							(*ammoiterbeg)->setTouchEnemy(TRUE);
							(*enemyiterbeg)->setTouchAmmo(TRUE);
							delete* enemyiterbeg;
							*enemyiterbeg = NULL;
							enemyiterbeg = SavedEnemy.erase(enemyiterbeg);
							Score++;
							break;
						}
						else //enemyiterbeg = SavedEnemy.begin(); 
							++enemyiterbeg;
					}
					++ammoiterbeg;
					Sleep(2);
				}
			}
			Sleep(2);
		}
		Sleep(2);
	}
}
inline void MouseLeftDownEvent() {
	ForegroundWindow = GetForegroundWindow();
	if (ForegroundWindow == MainWindow) {
		MouseLeftDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? TRUE : FALSE;
	}
	else MouseLeftDown = FALSE;
}
void DEBUG() {
	while(1){
		FpsTime = clock();
		FPS = 1000.0 / (FrameTimeEnd_Temp - FrameTimeStart_Temp);

		printf("[Debug Message]\n");
		printf("Time: %.3lfs | %.lfms\n\n", FpsTime/1000.0 , FpsTime);
		printf("FrameTime:%.0lfms\n", FrameTimeEnd_Temp - FrameTimeStart_Temp);
		printf("AllFrames:%d\n", FrameCount);
		printf("FPS:%.3lf\n", FPS);
		printf("\n[Mouse]\n");
		printf("MousePositon: X=%d,Y=%d\n", mousePoint.x, mousePoint.y);
		printf("MouseLeftDown:%d\n", MouseLeftDown);
		printf("\n[Game]\n");
		printf("Score:%d\n", Score);
		printf("\n[Ammo]\n");
		printf("SavedAmmoCount:%zd\n", SavedAmmo.size());
		printf("AllAmmoCount:%d\n", AllAmmoCount);
		printf("\n[Anemy]\n");
		printf("SavedEnemyCount:%zd\n", SavedEnemy.size());
		printf("AllEnemyCount:%d\n", AllEnemyCount);
		printf("EnmeySpeed:%d\n", RandEnemySpeed);
		printf("EnmeyX:%d\n", RandEnemyX);
		printf("EnmeyR:%d\n", RandEnemyR);
		printf("EnmeyTime:%d\n", RandEnemyTime);
		printf("EnmeyColorRGB:%d %d %d\n", RandEnemyColorR, RandEnemyColorG, RandEnemyColorB);

		Sleep(30);
		system("cls");
	}
}


