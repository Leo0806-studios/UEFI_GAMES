#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
#include <ANNOTATIONS.h>
/// <summary>
/// this is a simple physics engine that uses AABB collision detection and triggers callbacks when colliders collide.
/// it works on non owning (weak) pointers to colliders, so it does not own the memory of the colliders.
/// all of the colliders are owned by the objects they belong to
/// </summary>


/// <summary>
/// callbacktypedef. returns void and takes two non owning pointers to the colliders that collided.
/// self is the collider that triggered the callback, other is the collider that was collided with.
/// ths callback will allways be called for both colliders, so you can use it to trigger events on both colliders.
/// </summary>
typedef void(*TrigerCallBack)(void* self, void*other);

/// <summary>
/// collider struct, this is a simple AABB collider that has an origin and extends.
/// is sphere is special as it has its objPosition in the center instead of one corner.
/// </summary>
typedef struct {
	Vector2 extends;
	bool isShphere;//spherer is a specia lcollider as it has its objPosition in the center instead of one corner
	Vector2* ObjPosition;
	TrigerCallBack TriggerCallBack;
	bool DebugDrawCollider;
	void* object;
}Collider;

/// <summary>
/// inits the physics engine with a given capacity for colliders
/// will perform heap allocation.
/// </summary>
/// <param name="initCapacity"></param>
void InitPhysics();
/// <summary>
/// checks for aabb collision between the two colliders.
/// </summary>
/// <param name="a">non owning pointer to a</param>
/// <param name="b">non owning pointer to b</param>
/// <returns></returns>
bool AABBCheck(__NON_OWNING(Collider* a), __NON_OWNING(Collider* b));
/// <summary>
/// updates the physics engine, this will check for collisions and call the trigger callbacks if needed.
/// </summary>
/// <param name=""></param>
void UpdatePhysics(void);
/// <summary>
/// adds the collider to the physics engine, this will perform a heap allocation to store a pointer to the collider.
/// </summary>
/// <param name="collider"></param>
void AddtoPhysics(__NON_OWNING(Collider* collider),int slot);
/// <summary>
/// removes the collider from the physics engine, this will free the memory allocated for the pointer.
/// </summary>
void RemoveFromPhysics(int slot);
/// <summary>
/// shuts down the physics engine, this will free all memory allocated for the collider pointers and the physics engine itself.
/// </summary>
/// <param name=""></param>
void ShutdownPhysics(void);
