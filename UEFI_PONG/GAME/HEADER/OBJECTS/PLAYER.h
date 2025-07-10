#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
typedef struct {
	Vector2 position;
	Vector2 extends;
	bool player1;
}Player;
// retruns a non owning pointer to the player. the object is stored internaly
Player* CreatePlayer(bool Player1);
void DestroyPlayer(Player* player);
void UpdatePlayers();