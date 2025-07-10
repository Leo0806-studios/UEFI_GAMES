#pragma warning (push,0)
#include <gnu-efi/inc/efi.h>
#include <gnu-efi/inc/efilib.h>
#pragma warning(pop)
#include <../GAME/HEADER/PHYSICS/PHYSICS.h>
#include <../HEADER/HEAP/HEAP.h>
#include "../../HEADER/RENDER/RENDER.h"
static Collider* Colliders[3] = { 0 };
//handles special sphere case where we first need to calculate te corner from the obj position
// as the extends are not half extends we can just use the obj position as min
static Vector2 get_min(Collider* c) {
    Vector2 min = { 0 };
    if (c->isShphere) {
        Vector2 ActualPos = AddVector2(
            *c->ObjPosition,
            ScaleVector2(c->extends, -0.5)
        );
        min.x = ActualPos.x;
        min.y = ActualPos.y ;
    }
    else {
        min.x = c->ObjPosition->x;
        min.y= c->ObjPosition->y;
    }

    return min;
}

static Vector2 get_max(Collider* c) {
    Vector2 max = { 0 };
    if (c->isShphere) {
        Vector2 ActualPos = AddVector2(
            *c->ObjPosition,
            ScaleVector2(c->extends, -0.5)
        );
        max.x = ActualPos.x + c->extends.x;
        max.y =ActualPos.y + c->extends.y;

    }
    max.x = c->ObjPosition->x + c->extends.x;
    max.y = c->ObjPosition->y + c->extends.y;
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
    if (Colliders[0]->DebugDrawCollider) {
        DrawRectangle(*Colliders[0]->ObjPosition, Colliders[0]->extends.x-1, Colliders[0]->extends.y-1, 0xff0000);
    }
    if (Colliders[1]->DebugDrawCollider) {
        DrawRectangle(*Colliders[1]->ObjPosition, Colliders[1]->extends.x-1, Colliders[1]->extends.y-1, 0xff0000);


    }
    if (Colliders[2]->DebugDrawCollider) {

        Vector2 ActualPos = AddVector2(
            *Colliders[2]->ObjPosition,
            ScaleVector2(Colliders[2]->extends, -0.5)
        );
        DrawRectangle(ActualPos, Colliders[2]->extends.x, Colliders[2]->extends.y, 0xff0000);


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

void AddtoPhysics(Collider* collider,int slot)
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
