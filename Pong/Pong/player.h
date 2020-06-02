#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"

class Player : public Paddle
{
	public:
		Player(SDL_Renderer* r, int x, int w = 40, int h = 160);
		Player(int x = 0, int w = 40, int h = 160);
		~Player(void);

		void update(void);

	private:
		const Uint8* key;
};

#endif
