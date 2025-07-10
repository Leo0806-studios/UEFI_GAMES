#include "../../../HEADER/OBJECTS/PLAYER.h"
#include "../../../../HEADER/HEAP/HEAP.h"
#include "../../../HEADER/RENDER/RENDER.h"
#pragma warning (push,0)
#include <gnu-efi/inc/efilib.h>
#pragma warning(pop)
#include "../../../HEADER/INPUT/INPUT.h"
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
	player->Score = 0;
	player->collider = Alloc(sizeof(Collider));
	player->collider->ObjPosition = &player->position;
	player->collider->extends = size;
	player->collider->TriggerCallBack = PlayerCallback;
	player->collider->DebugDrawCollider = true;
	player->collider->object = player;
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
	//Print(L"\n Updating Players\n");
	//check if both exist
	if (Players[0] == NULLPTR || Players[1] == NULLPTR) {
		Print(L"A Playyer is null. 0: %d , 0: %d", Players[0], Players[1]);

		return;
	}
	//update both players.
	EFI_INPUT_KEY Key;
	GlobalST->ConIn->ReadKeyStroke(GlobalST->ConIn, &Key);
	switch (Key.ScanCode) {
	case SCAN_UP: {
		if ((Players[0]->position.y - 20) <= 0) {
			Players[0]->position.y =1;
			break;
		}
		Players[0]->position.y -= 20;

		break;
	}
	case SCAN_DOWN: {
		if ((Players[0]->position.y+Players[0]->extends.y + 20) >= GlobalFramebuffer.Height) {
			Players[0]->position.y = GlobalFramebuffer.Height- Players[0]->extends.y-20;
			break;
		}
		Players[0]->position.y += 20;

		break;
	}
	default: {
		break;
	}
	}

	EFI_INPUT_KEY Key2;
	GlobalST->ConIn->ReadKeyStroke(GlobalST->ConIn, &Key2);
	switch (Key.ScanCode) {
	case SCAN_F1: {
		if ((Players[1]->position.y - 20) <= 0) {
			Players[1]->position.y = 1;
			break;
		}
		Players[1]->position.y -= 20;

		break;
	}
	case SCAN_F2: {
		if ((Players[1]->position.y + Players[1]->extends.y +20) >= GlobalFramebuffer.Height) {
			Players[1]->position.y = GlobalFramebuffer.Height - Players[1]->extends.y - 20;
			break;
		}
		Players[1]->position.y += 20;

		break;
	}
	default: {
		break;
	}
	}
}

void DrawPlayers()
{
	if (Players[0] != NULLPTR)
	{
		DrawCircle(Players[0]->position, 5, 0xFF0000FF);
		DrawRectangle(Players[0]->position, Players[0]->extends.x,Players[0]->extends.y, 0xFF0000FF);
	}
	if (Players[1] != NULLPTR)
	{
		DrawCircle(Players[1]->position, 5, 0x0000FFFF);

		DrawRectangle(Players[1]->position, Players[1]->extends.x, Players[1]->extends.y, 0x0000FFFF);
	}
}

void IncrementScore(bool Player1)
{
	if (Player1) {
		Players[0]->Score++;
	}
	else {
		Players[1]->Score++;
	}
}

void PrintScores(void)
{
	Print(L"***** UEFI_PONG SCORES *****\n");
	Print(L"Player 1 %d\n", Players[0]->Score);
	Print(L"Player 2 %d\n", Players[1]->Score);
}
