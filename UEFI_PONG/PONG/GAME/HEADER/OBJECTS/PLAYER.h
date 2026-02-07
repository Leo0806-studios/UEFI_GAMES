#pragma once
#include <GLOBALS.h>
#include <DATATYPES/VEC2/VEC2.h>
#include <ANNOTATIONS.h>
#include "PHYSICS/PHYSICS.h"
#include "GAME_OBJECT/GAME_OBJECT.h"
typedef struct {
	GameObject Base;
	Vector2 extends;
	__OWNING(Collider*)collider;
	bool player1;
	size_t Score;
}Player;
/// <summary>
/// Creates player entities with the specified size.
/// </summary>
/// <param name="Size">The size to assign to each player, represented as a Vector2.</param>
void CreatePlayers(Vector2 Size);
void DestroyPlayer(Player* thisPtr);


void UpdatePlayer(Player* thisPtr, float deltaTime);
/// <summary>
/// Draws both players to the screen.  
/// </summary>
void DrawPlayers();
/// <summary>
/// incrementsd the score of a player.
/// if true increments of player1
/// </summary>
///  <param name="Player1"></param>
void IncrementScore(bool Player1);
/// <summary>
/// prints the score of both players to the console
///  </summary>
/// <param name=""></param>
void PrintScores(void);