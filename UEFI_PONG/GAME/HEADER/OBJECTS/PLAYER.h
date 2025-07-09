#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
typedef struct {
	Vector2 position;
	Vector2 extends;
	bool player1;
}Player;

Player* CreatePlayer(bool Player1);
void DestroyPlayer(Player* player);
