#include "../../../HEADER/OBJECTS/PLAYER.h"
#include "../../../../HEADER/HEAP/HEAP.h"
#include "../../../HEADER/RENDER/RENDER.h"
#include <gnu-efi/inc/efilib.h>
Player* Players[2] = { 0 };

static void PlayerCallback(void* this, void* other) {

}
/// <summary>
/// Creates and initializes a new Player object with the specified size and position.
/// </summary>
/// <param name="size">The size (extends) of the player as a Vector2.</param>
/// <param name="position">The initial position of the player as a Vector2.</param>
/// / <returns>A owning pointer to the newly created Player object.</returns>
static  __OWNING(Player*)CreatePlayer1(Vector2 size, Vector2 position,int playerslot)
{
	__OWNING(Player * player) = Alloc(sizeof(Player));
	player->position = position;
	player->extends = size;
	player->collider = Alloc(sizeof(Collider));
	player->collider->ObjPosition = &player->position;
	player->collider->extends = size;
	player->collider->TriggerCallBack = PlayerCallback;
	AddtoPhysics(player->collider, playerslot);
	return player;
}
void  CreatePlayers(Vector2 size)
{
	if(Players[0]==nullptr && Players[1] == nullptr)
	{
		Players[0] = CreatePlayer1(size, (Vector2){ GlobalFramebuffer.Width-100, 100 },1);
		Players[1] = CreatePlayer1(size, (Vector2){ 100, 100 },2);
	}
	else
	{
		Print(L"Players already created!\n");
	}

}

void DestroyPlayers(void)
{
}

void UpdatePlayers()
{
}

void DrawPlayers()
{
	if (Players[0] != NULLPTR)
	{
		DrawRectangle(Players[0]->position, Players[0]->extends.x,Players[0]->extends.y, 0xFF0000FF);
	}
	if (Players[1] != NULLPTR)
	{
		DrawRectangle(Players[1]->position, Players[1]->extends.x, Players[1]->extends.y, 0x0000FFFF);
	}
}
