#pragma once
#include<Windows.h>

class Enemy {
protected:
	bool enemy_exist;//�����Ƿ����
	bool touch_ammo;
	int enemy_x;//����λ��x
	int enemy_y;//����λ��y
	int enemy_r;
	COLORREF enemy_color;
	int speed;//���˵��ٶ�
	static int Count;//��������
	//double move_time;//�����ƶ�������
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