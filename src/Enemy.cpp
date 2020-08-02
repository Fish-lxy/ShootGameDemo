#pragma once

#include "Enemy.h"
#include "Ammo.h"
#include<graphics.h>
#include<Windows.h>
#include<cmath>

Enemy::Enemy(int spd, int x,int r) {
	enemy_exist = TRUE;
	touch_ammo = FALSE;
	speed = spd;
	enemy_x = x;
	enemy_y = 0;
	enemy_r = r;
}
void Enemy::move() {
	enemy_exist = isEnemyExist();
	if (enemy_exist == TRUE) {
		solidcircle(enemy_x, enemy_y, enemy_r);
		enemy_y += speed;
	}
}
bool Enemy::getExist() {
	return enemy_exist;
}
void Enemy::setTouchAmmo(bool touch) {
	touch_ammo = touch;
}

int Enemy::getX() {
	return enemy_x;
}
int Enemy::getY() {
	return enemy_y;
}
int Enemy::getR() {
	return enemy_r;
}
bool Enemy::isEnemyExist() {
	if (touch_ammo == TRUE)
		return FALSE;
	if (enemy_y >= 600)
		return FALSE;
	else
		return TRUE;
}