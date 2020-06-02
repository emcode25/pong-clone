#include "paddle.h"
#include "hints.h"

Paddle::Paddle(SDL_Renderer* r, int x, int w, int h)
{
	setRenderer(r);
	box = {x, WINHEIGHT / 2 - h, w, h};
	box.y -= box.h / 2;
}

Paddle::Paddle(int x, int w, int h)
{
	r = NULL;
	box = {x, WINHEIGHT / 2 - h, w, h};
	box.y -= box.h / 2;
}

Paddle::~Paddle(void)
{
	
}

SDL_Rect* Paddle::getRect(void)
{
	return &box;
}

void Paddle::setRenderer(SDL_Renderer* r)
{
	this->r = r;
}

void Paddle::update(SDL_Rect* b)
{
	box.y += (int) ((b->y + b->h / 2 > box.y + box.h / 2) ? speed : -speed);

	if(box.y < 0)
	{
		box.y += (int) speed;
	}
	else if(box.y + box.h > WINHEIGHT)
	{
		box.y -= (int) speed;
	}
}

void Paddle::render(void)
{
	if(r != NULL)
	{
		SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(r, &box);
	}
}