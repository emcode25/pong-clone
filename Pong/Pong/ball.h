#ifndef BALL_H
#define BALL_H

#include <SDL.h>

class Ball
{
	public:
		Ball(SDL_Renderer* r = NULL, int x = 0, int y = 0);
		~Ball(void);

		int update(SDL_Rect* p1, SDL_Rect* p2);
		void render(void);
		
		SDL_Rect* getRect(void);

	private:
		SDL_Renderer* r;
		SDL_Rect dot;
		bool isMovingLeft, isMovingUp;
		int xspeed = 7, yspeed = 5;
};

#endif