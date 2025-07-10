#include <../GAME/HEADER/PHYSICS/PHYSICS.h>
#include <../HEADER/HEAP/HEAP.h>
static Vector2 get_min(Collider* c) {
    Vector2 min = { 0 };
    min.x = c->origin.x - c->extends.x;
    min.y = c->origin.y - c->extends.y;
    return min;
}

static Vector2 get_max(Collider* c) {
    Vector2 max = { 0 };
    max.x = c->origin.x + c->extends.x;
    max.y = c->origin.y + c->extends.y;
    return max;
}
typedef struct {
     Collider** colliders ;
	 size_t count;
     size_t capacity;

}__physicsInternals;
static __physicsInternals physicsInternals = { 0 };
void InitPhysics(size_t initCapacity)
{
    physicsInternals.colliders = (Collider**)Alloc(sizeof(Collider*) * initCapacity);
    if (physicsInternals.colliders == NULL) {
        // Handle memory allocation failure
        return;
    }
    physicsInternals.count = 0;
	physicsInternals.capacity = initCapacity;
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


void UpdatePhysics(void)
{
}

void AddtoPhysics(Collider* collider)
{

}

void RemoveFromPhysics(Collider* collider)
{
}

void ShutdownPhysics(void)
{
}
