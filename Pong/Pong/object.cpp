#include "object.h"
#include <stdio.h>
#include <SDL_image.h>

Object::Object(SDL_Renderer* r)
{
	this->r = r;
	texture = NULL;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Object::~Object()
{
	free();
}

void Object::setRenderer(SDL_Renderer* r)
{
	this->r = r;
}

int Object::loadFile(SDL_Renderer* r, const char* path, SDL_bool colorKey)
{
	free();
	this->r = r;

	SDL_Surface* loaded = IMG_Load(path);
	if (loaded == NULL)
	{
		printf("Unable to load %s.\n", path);
		return 1;
	}

	SDL_SetColorKey(loaded, colorKey, SDL_MapRGB(loaded->format, 0xAA, 0xAA, 0xAA));

	texture = SDL_CreateTextureFromSurface(r, loaded);
	if (texture == NULL)
	{
		printf("Texture %s load failed. Error: %s\n", path, IMG_GetError());
		SDL_FreeSurface(loaded);
		return 2;
	}

	width = loaded->w;
	height = loaded->h;
	SDL_FreeSurface(loaded);

	return 0;
}

void Object::render(float x, float y, SDL_Rect* clip, SDL_Rect* dest, double angle, SDL_RendererFlip flip)
{
	SDL_Rect quad = { x, y, width, height };

	if (clip != NULL)
	{
		quad.w = clip->w;
		quad.h = clip->h;
	}

	if (dest != NULL)
	{
		quad.w = dest->w;
		quad.h = dest->h;
	}

	SDL_RenderCopyEx(r, texture, clip, &quad, angle, NULL, flip);
}

void Object::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	width = 0;
	height = 0;
}

int Object::getWidth()
{
	return width;
}

int Object::getHeight()
{
	return height;
}