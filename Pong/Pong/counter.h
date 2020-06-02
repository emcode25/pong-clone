#ifndef COUNTER_H
#define COUNTER_H

#include <SDL.h>
#include "object.h"

class Counter : public Object
{
	public:
		Counter(SDL_Renderer* r = NULL, int x = 0, int y = 0);
		~Counter(void);

		void increase(void);
		void render(void);
		int getNum(void);
		void setNum(int num);

	private:
		int num = 0;
		int width = 80;
		SDL_Rect currNum;
		SDL_Renderer* r;
};

#endif