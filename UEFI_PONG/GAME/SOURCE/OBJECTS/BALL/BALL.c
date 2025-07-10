#include "../../../HEADER/OBJECTS/BALL.h"
#include <gnu-efi/inc/efilib.h>
#include "../../../../HEADER/HEAP/HEAP.h"
#include "../../../HEADER/PHYSICS/PHYSICS.h"
void BallColliderCallBack(void* self, void* other) {
	Print(L" collision betwen self %d and other %d \n", self, other);
}
Ball* CreateBall(float x, float y, int radius, int vx, int vy) {
	Ball* ball = (Ball*)Alloc(sizeof(Ball));
	//PrintHeap();
	if (ball == NULL) {
		return NULL; // Handle memory allocation failure
	}
	ball->position.x = x;
	ball->position.y = y;
	ball->velocity.x = vx;
	ball->velocity.y = vy;
	ball->radius = radius;
	ball->collider = Alloc(sizeof(Collider));
	ball->collider->ObjPosition = &ball->position;
	ball->collider->isShphere = true;
	ball->collider->DebugDrawCollider = true;
	Vector2 extends = { 2 * radius,2 * radius };
	ball->collider->extends = extends;
	ball->collider->TriggerCallBack = BallColliderCallBack;
	AddtoPhysics(ball->collider, 3);
	Print(L"Created Ball at x: %f, Y: %f, with size: %d and vvelocity vx: %f, vy: %f \n", ball->position.x, ball->position.y, ball->radius, ball->velocity.x, ball->velocity.y);

	return ball;
}

 void DestroyBall(Ball* ball) {
	if (ball != NULL) {
		DeAlloc(ball); // Free the memory allocated for the ball
	}
}

ScreenBorder BallColidesWithBorder(Ball* ball) {
	if (ball->position.x <= 0 + ball->radius) return left;
	else if (ball->position.y <= 0 + ball->radius) return top;
	else if (ball->position.x >= GlobalFramebuffer.Width - ball->radius) return right;
	else if (ball->position.y >= GlobalFramebuffer.Height - ball->radius) return bottom;
	else return none;
}

bool BallCollidesWithScreenBounds(Ball* ball) {
	if (
		ball->position.x <= 0 + ball->radius ||
		ball->position.y <= 0 + ball->radius ||
		ball->position.x >= GlobalFramebuffer.Width - ball->radius ||
		ball->position.y >= GlobalFramebuffer.Height - ball->radius
		) return true;
	else return false;
}

 void DrawBall(Ball* ball) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Draw the ball using the render function
	DrawCircle(ball->position, ball->radius, 0xFFFFFFFF); // White color
}

 void ReflectBall(Ball* ball, Vector2 normal) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Reflect the ball's velocity based on the normal vector
	float dotProduct = ball->velocity.x * normal.x + ball->velocity.y * normal.y;
	ball->velocity.x -= 2 * dotProduct * normal.x;
	ball->velocity.y -= 2 * dotProduct * normal.y;
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
			break;
		}
		case left: {
			Print(L"ball refllecting of left");

			Vector2 set = { 0,-1 };
			ReflectBall(ball, set);
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
	ball->position.x += ball->velocity.x * deltaTime;
	ball->position.y += ball->velocity.y * deltaTime;

}

 void SetBallVelocity(Ball* ball, int vx, int vy) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Set the ball's velocity
	ball->velocity.x = vx;
	ball->velocity.y = vy;
}
