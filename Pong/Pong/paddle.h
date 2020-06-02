#ifndef PADDLE_H
#define PADDLE_H

#include <SDL.h>

class Paddle
{
	public:
		Paddle(SDL_Renderer* r, int x, int w = 40, int h = 160);
		Paddle(int x = 0, int w = 40, int h = 160);
		~Paddle(void);

		void setRenderer(SDL_Renderer* r);
		virtual void update(SDL_Rect* b);
		void render(void);

		SDL_Rect* getRect(void);

	protected:
		SDL_Rect box;
		SDL_Renderer* r;
		float speed = 4.5;
};

#endif