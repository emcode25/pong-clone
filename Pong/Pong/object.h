#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>

class Object
{
	public:
		Object(SDL_Renderer* r = NULL);
		~Object();

		int loadFile(SDL_Renderer* r, const char* path, SDL_bool colorKey = SDL_TRUE);

		void setRenderer(SDL_Renderer* r);
		void render(float x, float y, SDL_Rect* clip = NULL, SDL_Rect* dest = NULL, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);

		void free();

		int getWidth();
		int getHeight();

	protected:
		SDL_Renderer* r;
		SDL_Texture* texture;
		int width, height;
		float x, y;
};

#endif