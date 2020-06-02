#include "ball.h"
#include "hints.h"
#include <cmath>

Ball::Ball(SDL_Renderer* r, int x, int y)
{
	this->r = r;
	isMovingUp = bool(rand() % 2);
	isMovingLeft = bool(rand() % 2);
	dot = {WINWIDTH / 2, WINHEIGHT / 2, 40, 40};
	dot.x = WINWIDTH / 2 - dot.w;
	dot.y = rand() % WINHEIGHT - dot.h + 1;
}

Ball::~Ball(void)
{
	
}

SDL_Rect* Ball::getRect(void)
{
	return &dot;
}

int Ball::update(SDL_Rect* p1, SDL_Rect* p2)
{
	int code = 0;

	dot.x += (isMovingLeft) ? -xspeed : xspeed;
	dot.y += (isMovingUp) ? -yspeed : yspeed;

	if(SDL_HasIntersection(&dot, p1) || SDL_HasIntersection(&dot, p2))
	{
		isMovingLeft = !isMovingLeft;
		code = 1;
	}

	if(dot.y < 0 || dot.y + dot.h > WINHEIGHT)
	{
		isMovingUp = !isMovingUp;
		code = 2;
	}

	if(dot.x < 0 )
	{
		dot.x = WINWIDTH / 2 - dot.w;
		dot.y = rand() % WINHEIGHT - dot.h + 1;
		isMovingUp = bool(rand() % 2);
		isMovingLeft = bool(rand() % 2);
		code = 3;
	}
	else if (dot.x + dot.w > WINWIDTH)
	{
		dot.x = WINWIDTH / 2 - dot.w;
		dot.y = (rand() % (WINHEIGHT - dot.h * 2)) + dot.h;
		isMovingUp = bool(rand() % 2);
		isMovingLeft = bool(rand() % 2);
		code = 4;
	}

	return code;
}

void Ball::render(void)
{
	if(r != NULL)
	{
		SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(r, &dot);
	}
}