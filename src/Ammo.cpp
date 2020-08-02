#pragma once

#include "Ammo.h"
#include<graphics.h>
#include<Windows.h>
#include<cmath>


Ammo::Ammo(int spd,int r) {
	ammo_exist = TRUE;
	touch_enemy = FALSE;
	move_time = 1;
	speed = spd;
	ammo_r = r;
	GetCursorPos(&ammo_mouse);
	setAmmo_main_window();
	ScreenToClient(Ammo_main_window, &ammo_mouse);
	getSinCos(&ammo_mouse, &ammo_sin, &ammo_cos);
}
void Ammo::move() {
	ammo_x = (int)(((ammo_cos) * move_time) + (double)CannonLength * (ammo_cos) + 400);
	ammo_y = (int)(((-ammo_sin) * move_time) + (double)CannonLength * (-ammo_sin) + 600);

	ammo_exist = isAmmoExist();
	if (ammo_exist == TRUE) {
		solidcircle(ammo_x, ammo_y, ammo_r);
		move_time += speed;
	}
}
bool Ammo::getExist() {
	return ammo_exist;
}
void Ammo::setTouchEnemy(bool touch) {
	touch_enemy = touch;
}
int Ammo::getX() {
	return ammo_x;
}
int Ammo::getY() {
	return ammo_y;
}
int Ammo::getR() {
	return ammo_r;
}

void Ammo::getSinCos(POINT* Point, double* getSin, double* getCos) {
	double adjacentside = (Point->x) - 400L;//ÁÚ±ß
	double hypotenuse = distancePoints((Point->x), (Point->y), 400, 600);//Ð±±ß
	double oppositeside = 600L - (Point->y);//¶Ô±ß
	ammo_cos = adjacentside / hypotenuse;
	ammo_sin = oppositeside / hypotenuse;
}
inline double Ammo::distancePoints(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
inline void Ammo::setAmmo_main_window() {
	Ammo::Ammo_main_window =FindWindow(L"EasyXWnd", L"ShootGameDemo");
}
inline bool Ammo::isAmmoExist() {
	if (touch_enemy == TRUE)
		return FALSE;
	if (ammo_x >= 800 || ammo_x <= 0)
		return FALSE;
	if (ammo_y >= 590 || ammo_y <= 0)
		return FALSE;
	else
		return TRUE;
}

