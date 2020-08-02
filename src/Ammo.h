#pragma once

#include<Windows.h>
extern const int CannonLength;//大炮的长度
class Ammo {
private:
	bool ammo_exist;//子弹是否存在
	bool touch_enemy;
	int ammo_x;//子弹位置x
	int ammo_y;//子弹位置y
	int ammo_r;
	int speed;//子弹的速度
	static int Count;//子弹总数
	HWND Ammo_main_window; //窗口句柄，计算坐标用
	POINT ammo_mouse;//子弹生成时的鼠标坐标
	double ammo_sin, ammo_cos;//子弹生成时的正弦余弦数；
	double move_time;//子弹移动计数器

public:
	Ammo(int spd, int r);
	void move();
	bool getExist();
	void setTouchEnemy(bool exist);
	int getX();
	int getY();
	int getR();
private:
	//类内部函数
	void getSinCos(POINT* Point, double* getSin, double* getCos);
	inline double distancePoints(int x1, int y1, int x2, int y2);
	inline void setAmmo_main_window();
	inline bool isAmmoExist();
};
