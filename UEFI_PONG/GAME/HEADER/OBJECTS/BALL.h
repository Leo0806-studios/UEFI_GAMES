#pragma once
#include <GLOBALS.h>
#include <../GAME/HEADER/DATATYPES/VEC2/VEC2.h>
#include <../HEADER/HEAP/HEAP.h>
#include <../GAME/HEADER/RENDER/RENDER.h>
#include "../PHYSICS/PHYSICS.h"
typedef struct Ball {
	Vector2 position; // Position of the ball
	Vector2 velocity; // Velocity of the ball
	int radius;    // Radius of the ball
	__OWNING(Collider*)collider;
} Ball;
/// <summary>
/// Creates a ball at the specifed location with the specified sze and location
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="radius"></param>
/// <param name="vx"></param>
/// <param name="vy"></param>
/// <returns></returns>
Ball* CreateBall(float x, float y, int radius, int vx, int vy);
/// <summary>
/// destroys the all and dealocates the memory
/// </summary>
/// <param name="ball"></param>
void DestroyBall(__THIS_PTR(Ball*)ball);

typedef enum {
	none,left,right,top,bottom
}ScreenBorder;
/// <summary>
/// checks with what screen border the ball collides ( or none)
/// </summary>
/// <param name="ball"></param>
/// <returns></returns>
ScreenBorder BallColidesWithBorder(Ball* ball);
/// <summary>
/// returns true if the ball collides with any screen border
/// </summary>
/// <param name="ball"></param>
/// <returns></returns>
bool BallCollidesWithScreenBounds(Ball* ball);
 /// <summary>
 /// Draws the ball to the screen
 /// </summary>
 /// <param name="ball"></param>
 void DrawBall(Ball* ball);
/// <summary>
/// reflects the ball on the provided normal
/// </summary>
/// <param name="ball"></param>
/// <param name="normal"></param>
void ReflectBall(Ball* ball, Vector2 normal);
/// <summary>
/// unused
/// </summary>
static bool edgeFlag = FALSE;
/// <summary>
/// updates the ball.
/// movement reflectin etc
/// </summary>
/// <param name="ball"></param>
/// <param name="deltaTime"></param>
void UpdateBall(Ball* ball, float deltaTime);
/// <summary>
/// manualy sets the balls velocity
/// </summary>
/// <param name="ball"></param>
/// <param name="vx"></param>
/// <param name="vy"></param>
void SetBallVelocity(Ball* ball, int vx, int vy);
/// <summary>
/// Sets the position of the ball.
/// </summary>
/// <param name="pos">The new position of the ball as a 2D vector.</param>
void SetBallPosition(Ball* ball,Vector2 pos);
