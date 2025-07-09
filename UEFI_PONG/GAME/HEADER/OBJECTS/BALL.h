#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
#include <../HEADER/HEAP/HEAP.h>
#include <../GAME/HEADER/RENDER/RENDER.h>

typedef struct Ball {
	Vector2 position; // Position of the ball
	Vector2 velocity; // Velocity of the ball
	int radius;    // Radius of the ball
} Ball;
Ball* CreateBall(int x, int y, int radius, int vx, int vy) {
	Ball* ball = (Ball*)Alloc(sizeof(Ball));
	if (ball == NULL) {
		return NULL; // Handle memory allocation failure
	}
	ball->position.x = x;
	ball->position.y = y;
	ball->velocity.x = vx;
	ball->velocity.y = vy;
	ball->radius = radius;
	return ball;
}

inline void DestroyBall(Ball* ball) {
	if (ball != NULL) {
		DeAlloc(ball); // Free the memory allocated for the ball
	}
}
inline void DrawBall(Ball* ball) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Draw the ball using the render function
	DrawCircle(ball->position, ball->radius, 0xFFFFFFFF); // White color
}
inline void ReflectBall(Ball* ball, Vector2 normal) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Reflect the ball's velocity based on the normal vector
	float dotProduct = ball->velocity.x * normal.x + ball->velocity.y * normal.y;
	ball->velocity.x -= 2 * dotProduct * normal.x;
	ball->velocity.y -= 2 * dotProduct * normal.y;
}
inline void UpdateBall(Ball* ball, float deltaTime) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Update the ball's position based on its velocity and the time delta
	ball->position.x += ball->velocity.x * deltaTime;
	ball->position.y += ball->velocity.y * deltaTime;
}
inline void SetBallVelocity(Ball* ball, int vx, int vy) {
	if (ball == NULL) {
		return; // Handle null pointer
	}
	// Set the ball's velocity
	ball->velocity.x = vx;
	ball->velocity.y = vy;
}
