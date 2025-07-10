#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
#include <ANNOTATIONS.h>
#include "../PHYSICS/PHYSICS.h"
typedef struct {
	Vector2 position;
	Vector2 extends;
	__OWNING(Collider*)collider;
	bool player1;
}Player;
/// <summary>
/// Creates player entities with the specified size.
/// </summary>
/// <param name="Size">The size to assign to each player, represented as a Vector2.</param>
void CreatePlayers(Vector2 Size);
/// <summary>
/// Destroys both players.
/// </summary>
/// <param name=""></param>
void DestroyPlayers(void);
/// <summary>
/// opdates both players.
/// polls input, updates location etc
/// </summary>
void UpdatePlayers();
/// <summary>
/// Draws both players to the screen.  
/// </summary>
void DrawPlayers();