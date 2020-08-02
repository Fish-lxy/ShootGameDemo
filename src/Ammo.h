#pragma once

#include<Windows.h>
extern const int CannonLength;//���ڵĳ���
class Ammo {
private:
	bool ammo_exist;//�ӵ��Ƿ����
	bool touch_enemy;
	int ammo_x;//�ӵ�λ��x
	int ammo_y;//�ӵ�λ��y
	int ammo_r;
	int speed;//�ӵ����ٶ�
	static int Count;//�ӵ�����
	HWND Ammo_main_window; //���ھ��������������
	POINT ammo_mouse;//�ӵ�����ʱ���������
	double ammo_sin, ammo_cos;//�ӵ�����ʱ��������������
	double move_time;//�ӵ��ƶ�������

public:
	Ammo(int spd, int r);
	void move();
	bool getExist();
	void setTouchEnemy(bool exist);
	int getX();
	int getY();
	int getR();
private:
	//���ڲ�����
	void getSinCos(POINT* Point, double* getSin, double* getCos);
	inline double distancePoints(int x1, int y1, int x2, int y2);
	inline void setAmmo_main_window();
	inline bool isAmmoExist();
};
