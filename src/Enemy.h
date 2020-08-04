#pragma once
#include<Windows.h>

class Enemy {
protected:
	bool enemy_exist;//敌人是否存在
	bool touch_ammo;
	int enemy_x;//敌人位置x
	int enemy_y;//敌人位置y
	int enemy_r;
	COLORREF enemy_color;
	int speed;//敌人的速度
	static int Count;//敌人总数
	//double move_time;//敌人移动计数器
public:
	Enemy(int spd, int x, int r, COLORREF colorrgb);
	void move();
	bool getExist();
	void setTouchAmmo(bool touch);
	int getX();
	int getY();
	int getR();
private:
	bool isEnemyExist();
};