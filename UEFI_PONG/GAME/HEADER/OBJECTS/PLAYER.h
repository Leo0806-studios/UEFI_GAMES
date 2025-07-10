#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
#include <ANNOTATIONS.h>
typedef struct {
	Vector2 position;
	Vector2 extends;
	bool player1;
}Player;
/// <summary>
/// creates either player 1 or player 2 based on the boolean.
/// returns a non owning pointer to the created player. the actual object is stored internaly
/// </summary>
/// <param name="Player1"></param>
/// <returns></returns>
__NON_OWNING(Player*)CreatePlayer(bool Player1);
/// <summary>
/// destroys eithter player 1 or player 2 based on the boolean
/// </summary>
/// <param name="player1"></param>
void DestroyPlayer(bool player1);
/// <summary>
/// opdates both players.
/// polls input, updates location etc
/// </summary>
void UpdatePlayers();
/// <summary>
/// Draws both players to the screen.  
/// </summary>
void DrawPlayers();