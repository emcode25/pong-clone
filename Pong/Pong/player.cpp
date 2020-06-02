#include "player.h"
#include "hints.h"

Player::Player(SDL_Renderer* r, int x, int w, int h) : Paddle(r, x, w, h)
{
	//this->Paddle::Paddle(r, x, w, h); //WTF WHY MUST I DO "THIS"
	key = SDL_GetKeyboardState(NULL);
	speed = 5;
}

Player::Player(int x, int w, int h)
{
	Paddle::Paddle(x, w, h);
	key = SDL_GetKeyboardState(NULL);
}

Player::~Player(void)
{
	
}

void Player::update(void)
{
	box.y += (int) ((key[SDL_SCANCODE_DOWN] - key[SDL_SCANCODE_UP]) * speed);

	if (box.y < 0)
	{
		box.y += (int) speed;
	}
	else if (box.y + box.h > WINHEIGHT)
	{
		box.y -= (int) speed;
	}
}