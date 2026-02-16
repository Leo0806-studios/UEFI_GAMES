#pragma warning (push,0)
#include <efi.h>
#include <efilib.h>
#pragma warning(pop)
#include <PHYSICS/PHYSICS.h>
#include <HEAP/HEAP.h>
#include "RENDER/RENDER.h"
typedef struct {
	__NON_OWNING(Collider*)* collider;
	size_t count, capacity;
}ColiderList;
static ColiderList GlobalColiderList;
static Collider** AppendToList(ColiderList* thisPtr, Collider* newObj) {
	if (thisPtr->count == thisPtr->capacity) {
		Collider** tmp = Alloc(sizeof(Collider*) * thisPtr->count * 2);
		for (size_t index = 0; index < thisPtr->count; index++) {
			tmp[index] = thisPtr->collider[index];
		}
		DeAlloc(thisPtr->collider);
		thisPtr->collider = tmp;
		thisPtr->capacity = thisPtr->capacity * 2;
	}
	thisPtr->collider[thisPtr->count] = newObj;
	thisPtr->count++;
	return &thisPtr->collider[thisPtr->count - 1];

}
static void RemoveFromListVal(ColiderList* thisPtr, Collider* val) {

}
static void RemoveFromListIndex(ColiderList* thisPtr, size_t index) {

}

//handles special sphere case where we first need to calculate te corner from the obj position
// as the extends are not half extends we can just use the obj position as min
static Vector2 get_min(const Collider* c) {
	Vector2 min = { 0 };
	if (c->isShphere) {
		min = AddVector2(
			c->object->Position,
			ScaleVector2(c->extends, -0.5)
		);

	}
	else {
		min = c->object->Position;
	}

	return min;
}

static Vector2 get_max(Collider* c) {
	Vector2 max = { 0 };
	if (c->isShphere) {
		Vector2 ActualPos = AddVector2(
			c->object->Position,
			ScaleVector2(c->extends, -0.5)
		);
		max = AddVector2(
			ActualPos,
			c->extends
		);


	}
	else {
		max = AddVector2(
			c->object->Position,
			c->extends
		);
	}

	return max;
}

void InitPhysics()
{
	Print(L"Initializing Physics");

}
bool AABBCheck(Collider* a, Collider* b)
{

	Vector2 a_min = get_min(a);
	Vector2 a_max = get_max(a);
	Vector2 b_min = get_min(b);
	Vector2 b_max = get_max(b);

	bool x_overlap = (a_min.x <= b_max.x) && (a_max.x >= b_min.x);
	bool y_overlap = (a_min.y <= b_max.y) && (a_max.y >= b_min.y);

	return x_overlap && y_overlap;
}

void DrawDebugColliders() {
	for (size_t i = 0; i < GlobalColiderList.count; i++) {
		if (GlobalColiderList.collider[i]->DebugDrawCollider) {
			if (GlobalColiderList.collider[i]->isShphere) {

				Vector2 ActualPos = AddVector2(
					*GlobalColiderList.collider[i]->ObjPosition,
					ScaleVector2(GlobalColiderList.collider[i]->extends, -0.5)
				);
				DrawRectangle(ActualPos, GlobalColiderList.collider[i]->extends.x, GlobalColiderList.collider[i]->extends.y, 0xff0000);
			}
			DrawRectangle(GlobalColiderList.collider[i]->object->Position, GlobalColiderList.collider[i]->extends.x, GlobalColiderList.collider[i]->extends.y, 0xff0000);
		}
	}
}
void UpdatePhysics(void)
{
	if ((Colliders[0] == NULLPTR) ||
		(Colliders[1] == NULLPTR) ||
		(Colliders[2] == NULLPTR)) {
		Print(L" at least on of the elements in physics slots is null 0: %d 1: %d 2: %d", Colliders[0], Colliders[1], Colliders[2]);
		return;
	}


	//draws debug colliders on objects that have it enabled
	DrawDebugColliders();
	//assume that 1 and 2 are the padels. they will never interact. so i will only need to ckeck if 1&3 collide and 2&3



	//1&3 check
	if (AABBCheck(Colliders[0], Colliders[2])) {
		Colliders[0]->TriggerCallBack(Colliders[0], Colliders[2]);
		Colliders[2]->TriggerCallBack(Colliders[2], Colliders[0]);
	}

	//2&3 check
	if (AABBCheck(Colliders[1], Colliders[2])) {
		Colliders[1]->TriggerCallBack(Colliders[1], Colliders[2]);
		Colliders[2]->TriggerCallBack(Colliders[2], Colliders[1]);
	}

}

void AddtoPhysics(Collider* collider, int slot)
{
	if (!collider) {
		Print(L"Passed Nullptr to AddTo Physics");
		return;
	}
	if (slot > 3) {
		Print(L"can add %d to slot %d as it larger than the max allowed (3)", collider, slot);
		return;
	}
	if (slot <= 0) {
		Print(L" cant add %d to slot %d. %d must be larger than 0", collider, slot, slot);
		return;
	}
	if (Colliders[slot - 1] != NULLPTR) {
		Print(L"cant add %d to slot %d as it is allready used", collider, slot);
		return;
	}
	Colliders[slot - 1] = collider;




}

void RemoveFromPhysics(int slot)
{
	if (slot > 3 || slot <= 0) {
		Print(L"Cant remove from slot %d as the specified slot is outside the allowed range of 1-3 (inclusive)", slot);
		return;
	}
	if (Colliders[slot - 1] == NULLPTR) {
		Print(L"trying to remove a element from the physics simulation that was allready removed.\n currently this is not an isiue as its a static array but in the future this will crash.");
		return;

	}
	Colliders[slot - 1] = NULLPTR;
}



void ShutdownPhysics(void)
{
	Print(L"Shuting down Physics");
}
