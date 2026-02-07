#include "../../../HEADER/OBJECTS/BALL.h"
#pragma warning(push,0)
#include <efilib.h>
#pragma warning(pop)
#include "HEAP/HEAP.h"
#include "PHYSICS/PHYSICS.h"
#include "OBJECTS/PLAYER.h"
void BallColliderCallBack(void* self, void* other) {
	Print(L" collision betwen self %d and other %d \n", self, other);
	Ball* ball = ((Ball*)((Collider*)self)->object);
	Collider* othercol = (Collider*)other;
	Vector2 Normal = { 0 };
	if (((unsigned long long)othercol->object->Position.x) > (GlobalFramebuffer.Width / 2)) {
		Normal.x = 1;
		Normal.y = 0;

	}
	else {
		Normal.x = -1;
		Normal.y = 0;
	}
	ReflectBall(ball, Normal);

}
Ball* CreateBall(float x, float y, int radius, int vx, int vy) {
	Ball* ball = (Ball*)Alloc(sizeof(Ball));
	if (ball == NULL) {
		return NULL; // Handle memory allocation failure
	}
	auto a = DestroyBall;
	ball->Base = (GameObject){.Position=(Vector2){.x = x,.y = y},.VTable = (vTable) {.Destroy = &DestroyBall,.Update = &UpdateBall}};
	ball->velocity = (Vector2){ .x = vx,.y = vy };
	ball->radius = radius;
	ball->collider = Alloc(sizeof(Collider));
	*(ball->collider) = (Collider) {
			.isShphere = true,
			.DebugDrawCollider = true,
			.extends = { 2 * radius,2 * radius },
			.TriggerCallBack = BallColliderCallBack,
			.object = ball
	};
	AddtoPhysics(ball->collider, 3);
	Print(L"Created Ball at x: %f, Y: %f, with size: %d and vvelocity vx: %f, vy: %f \n", ball->Base.Position.x, ball->Base.Position.y, ball->radius, ball->velocity.x, ball->velocity.y);

	return ball;
}

void DestroyBall(Ball* ball) {
	if (ball != NULL) {
		DeAlloc(ball); // Free the memory allocated for the ball
	}
}

ScreenBorder BallColidesWithBorder(const Ball* ball) {
	if ((double)ball->Base.Position.x <= 0 + ball->radius) return left;
	else if (ball->Base.Position.y <= 0 + ball->radius) return top;
	else if (ball->Base.Position.x >= GlobalFramebuffer.Width - ball->radius) return right;
	else if (ball->Base.Position.y >= GlobalFramebuffer.Height - ball->radius) return bottom;
	else return none;
}

bool BallCollidesWithScreenBounds(Ball* ball) {
	if (
		ball->Base.Position.x <= 0 + ball->radius ||
		ball->Base.Position.y <= 0 + ball->radius ||
		ball->Base.Position.x >= GlobalFramebuffer.Width - ball->radius ||
		ball->Base.Position.y >= GlobalFramebuffer.Height - ball->radius
		) return true;
	else return false;
}

void DrawBall(Ball* ball) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Draw the ball using the render function
	DrawCircle(ball->Base.Position, ball->radius, 0xFFFFFFFF); // White color
}

void ReflectBall(Ball* ball, Vector2 normal) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	//add rng to the reflected angle 



	// Reflect the ball's velocity based on the normal vector
	float dotProduct = ball->velocity.x * normal.x + ball->velocity.y * normal.y;
	ball->velocity.x -= 2 * dotProduct * normal.x;
	ball->velocity.y -= 2 * dotProduct * normal.y;
	///scales ball velocity to be higher after each bounce

}

void UpdateBall(Ball* ball, float deltaTime) {
	if (ball == NULL) {

		return; // Handle null pointer
	}
	if (BallCollidesWithScreenBounds(ball) && !edgeFlag) {
		ball->velocity.x *= -1;
		ball->velocity.y *= -1;
		switch (BallColidesWithBorder(ball)) {
			case right: {
				Print(L"ball refllecting of right");

				Vector2 set = { 0,1 };
				ReflectBall(ball, set);
				IncrementScore(false);
				SetBallPosition(ball, (Vector2) { 200, 100 });
				break;
			}
			case left: {
				Print(L"ball refllecting of left");

				Vector2 set = { 0,-1 };
				ReflectBall(ball, set);
				IncrementScore(true);
				SetBallPosition(ball, (Vector2) { 200, 100 });
				break;
			}
			case top: {
				Print(L"ball refllecting of top");
				Vector2 set = { 1,0 };
				ReflectBall(ball, set);
				break;
			}
			case bottom: {
				Print(L"ball refllecting of bottom");

				Vector2 set = { -1,0 };
				ReflectBall(ball, set);
				break;
			}
		}
		deltaTime = deltaTime * 2;
	}
	// Update the ball's position based on its velocity and the time delta
	ball->Base.Position.x += ball->velocity.x * deltaTime;
	ball->Base.Position.y += ball->velocity.y * deltaTime;

}

void SetBallVelocity(Ball* ball, int vx, int vy) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Set the ball's velocity
	ball->velocity.x = vx;
	ball->velocity.y = vy;
}

void SetBallPosition(Ball* ball, Vector2 pos)
{
	ball->Base.Position = pos;
}
