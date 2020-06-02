#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "object.h"
#include "player.h"
#include "ball.h"
#include "counter.h"
#include "hints.h"

enum class TextAlignment
{
	LEFT,
	RIGHT,
	CENTER
};

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Player player;
Paddle ai;
Ball dot;
Object background;
Counter count1, count2;
Mix_Chunk* hit;
Mix_Chunk* score;
Mix_Chunk* wall;
TTF_Font* titleFont;
TTF_Font* regularFont;

void printscr(SDL_Renderer* r, int x, int y, const char* string, TTF_Font* font, SDL_Color color = { 0, 0, 0 });
void printscr(SDL_Renderer* r, int x, int y, char* string, TTF_Font* font, TextAlignment align = TextAlignment::LEFT, SDL_Color color = { 0, 0, 0 });

int init(void);
int load(void);
int game(void);
void loop(SDL_Event* e);
void close(void);

int main(int argc, char** argv)
{
	int code = init();
	if (code != 0)
	{
		printf("Init failed. Code: %d\n", code);
		goto end;
	}

	code = load();
	if (code != 0)
	{
		printf("Loading failed. Code: %d\n", code);
		goto end;
	}

	game();

	end:
	close();
	return code;
}

int init(void)
{
	int flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER;
	int code = SDL_Init(flags);
	if (code != 0)
	{
		printf("SDL failed to initialize. Code: %d\n", code);
		return 1;
	}

	window = SDL_CreateWindow("Bothersome", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINWIDTH, WINHEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window creation failed.\n");
		return 2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer creation failed.\n");
		return 3;
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

	flags = IMG_INIT_PNG | IMG_INIT_JPG;
	code = IMG_Init(flags);
	if (!(code & flags))
	{
		printf("SDL_image failed to initialize. Code: %d\n", code);
		return 4;
	}

	flags = MIX_INIT_MP3 | MIX_INIT_OGG;
	code = Mix_Init(flags);
	if (!(code & flags))
	{
		printf("SDL_mixer failed to initialize. Code: %d\n", code);
		return 5;
	}

	code = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
	if (code != 0)
	{
		printf("SDL_mixer failed to open audio. Code: %d", code);
		return 6;
	}

	code = TTF_Init();
	if (code != 0)
	{
		printf("SDL_ttf failed to initialize. Code: %d\n", code);
		return 7;
	}

	srand((unsigned int) time(NULL));

	return 0;
}

int load(void)
{
	dot = Ball(renderer);
	player = Player(renderer, 40);
	ai = Paddle(renderer, WINWIDTH - 80);

	count1 = Counter(renderer, WINWIDTH * 3 / 8, 20);
	count1.loadFile(renderer, "data/numbers.png");
	count2 = Counter(renderer, WINWIDTH * 5 / 8, 20);
	count2.loadFile(renderer, "data/numbers.png");

	background = Object(renderer);
	int code = background.loadFile(renderer, "data/back.png", SDL_FALSE);

	if(code != 0)
	{
		return 1;
	}

	titleFont = TTF_OpenFont("data/ChicagoFLF.ttf", 72);
	regularFont = TTF_OpenFont("data/ChicagoFLF.ttf", 32);

	hit = Mix_LoadWAV("data/hit.wav");
	score = Mix_LoadWAV("data/score.wav");
	wall = Mix_LoadWAV("data/wall.wav");

	return 0;
}

int pause(SDL_Event* e)
{
	int boxw = WINWIDTH / 5;
	int boxh = WINHEIGHT / 16;

	SDL_Rect screen = {0, 0, WINWIDTH, WINHEIGHT};
	SDL_Rect contBox = { WINWIDTH / 2 - boxw / 2, WINHEIGHT * 4 / 6 - boxh / 2, boxw, boxh };
	SDL_Rect quitBox = { WINWIDTH / 2 - boxw / 2, WINHEIGHT * 5 / 6 - boxh / 2, boxw, boxh };

	SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
	SDL_RenderFillRect(renderer, &screen);

	char pauseText[] = "Paused";
	char contText[] = "Continue";
	char quitText[] = "Quit";

	printscr(renderer, WINWIDTH / 2, WINHEIGHT / 4, pauseText, titleFont, TextAlignment::CENTER);

	SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_RenderFillRect(renderer, &contBox);

	printscr(renderer, WINWIDTH / 2, contBox.y + boxh / 2, contText, regularFont, TextAlignment::CENTER);

	SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_RenderFillRect(renderer, &quitBox);

	printscr(renderer, WINWIDTH / 2, quitBox.y + boxh / 2, quitText, regularFont, TextAlignment::CENTER);

	SDL_RenderPresent(renderer);

	while (true)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return -1;
			}

			if (e->type == SDL_KEYDOWN)
			{
				switch (e->key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return 0;
				}
			}

			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_Rect mouse = { x, y, 1, 1 };

				switch (e->button.button)
				{
					case SDL_BUTTON_LEFT:
						if (SDL_HasIntersection(&contBox, &mouse))
						{
							return 0;
						}

						if (SDL_HasIntersection(&quitBox, &mouse))
						{
							return -1;
						}
				}
			}
		}
	}
}

void loop(SDL_Event* e)
{
	start:
	int boxw = WINWIDTH / 5;
	int boxh = WINHEIGHT / 16;

	SDL_Rect screen = { 0, 0, WINWIDTH, WINHEIGHT };
	SDL_Rect contBox = { WINWIDTH / 2 - boxw / 2, WINHEIGHT * 4 / 6 - boxh / 2, boxw, boxh };
	SDL_Rect quitBox = { WINWIDTH / 2 - boxw / 2, WINHEIGHT * 5 / 6 - boxh / 2, boxw, boxh };

	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	Uint32 dt = SDL_GetTicks();
	Uint32 skip = dt;
	int hitCode = 0;

	count1.setNum(0);
	count1.setNum(0);

	while(true)
	{
		while(SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return;
			}

			if(e->type == SDL_KEYDOWN)
			{
				switch(e->key.keysym.sym)
				{
					case SDLK_ESCAPE:
						int code = pause(e);

						if(code == -1)
						{
							return;
						}
				}
			}
		}

		do
		{
			skip = SDL_GetTicks() - dt;
		} 
		while(skip == 0);

		hitCode = dot.update(player.getRect(), ai.getRect());
		player.update();
		ai.update(dot.getRect());


		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		background.render(-1, -1);
		dot.render();
		player.render();
		ai.render();

		count1.render();
		count2.render();

		if(hitCode == 1)
		{
			Mix_PlayChannel(-1, hit, 0);
		}
		else if(hitCode == 2)
		{
			Mix_PlayChannel(-1, wall, 0);
		}
		else if(hitCode == 3 || hitCode == 4)
		{
			Mix_PlayChannel(-1, score, 0);

			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			background.render(-1, -1);
			player.render();
			ai.render();

			(hitCode == 3) ? count2.increase() : count1.increase();

			count1.render();
			count2.render();

			if(count1.getNum() == 10 || count2.getNum() == 10)
			{
				SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
				SDL_RenderFillRect(renderer, &screen);

				char winText[] = "You Win!";
				char loseText[] = "You Lose!";
				char contText[] = "Continue";
				char quitText[] = "Quit";

				if(count1.getNum() == 10)
				{
					printscr(renderer, WINWIDTH / 2, WINHEIGHT / 4, winText, titleFont, TextAlignment::CENTER);
				}
				else
				{
					printscr(renderer, WINWIDTH / 2, WINHEIGHT / 4, loseText, titleFont, TextAlignment::CENTER);
				}

				SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
				SDL_RenderFillRect(renderer, &contBox);

				printscr(renderer, WINWIDTH / 2, contBox.y + boxh / 2, contText, regularFont, TextAlignment::CENTER);

				SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
				SDL_RenderFillRect(renderer, &quitBox);

				printscr(renderer, WINWIDTH / 2, quitBox.y + boxh / 2, quitText, regularFont, TextAlignment::CENTER);

				SDL_RenderPresent(renderer);

				goto endstate;
			}
			else
			{
				SDL_Delay(1000);
			}
		}

		SDL_RenderPresent(renderer);

		dt = SDL_GetTicks();
	}

	endstate:
	while (true)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return;
			}

			if (e->type == SDL_KEYDOWN)
			{
				switch (e->key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return;
				}
			}

			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_Rect mouse = { x, y, 1, 1 };

				switch (e->button.button)
				{
				case SDL_BUTTON_LEFT:
					if (SDL_HasIntersection(&contBox, &mouse))
					{
						goto start;
					}

					if (SDL_HasIntersection(&quitBox, &mouse))
					{
						return;
					}
				}
			}
		}
	}
}

int game(void)
{
	SDL_Event e;
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	Uint32 dt = SDL_GetTicks();
	Uint32 skip = dt;

	char titleText[] = "PONG";
	char startText[] = "Start";
	char quitText[] = "Quit";

	int boxw = WINWIDTH / 5;
	int boxh = WINHEIGHT / 16;

	SDL_Rect startBox = {WINWIDTH / 2 - boxw / 2, WINHEIGHT * 4 / 6 - boxh / 2, boxw, boxh};
	SDL_Rect quitBox = {WINWIDTH / 2 - boxw / 2, WINHEIGHT * 5 / 6 - boxh / 2, boxw, boxh}; //FIX MATH (positioning)

	while (true)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				return 0;
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return 0;
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_Rect mouse = { x, y, 1, 1 };

				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT:
						if (SDL_HasIntersection(&startBox, &mouse))
						{
							loop(&e);
							return 0;
						}
							
						if(SDL_HasIntersection(&quitBox, &mouse))
						{
							return 0;
						}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
		SDL_RenderClear(renderer);

		printscr(renderer, WINWIDTH / 2, WINHEIGHT / 4, titleText, titleFont, TextAlignment::CENTER);

		SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
		SDL_RenderFillRect(renderer, &startBox);

		printscr(renderer, WINWIDTH / 2, startBox.y + boxh / 2, startText, regularFont, TextAlignment::CENTER);

		SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
		SDL_RenderFillRect(renderer, &quitBox);

		printscr(renderer, WINWIDTH / 2, quitBox.y + boxh / 2, quitText, regularFont, TextAlignment::CENTER);

		SDL_RenderPresent(renderer);
	}

	return 0;
}

void close(void)
{
	Mix_CloseAudio();

	Mix_FreeChunk(hit);
	Mix_FreeChunk(score);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void printscr(SDL_Renderer* r, int x, int y, const char* string, TTF_Font* font, SDL_Color color)
{
	if (r == NULL)
	{
		printf("No renderer loaded.");
		return;
	}

	if (font == NULL)
	{
		printf("No font loaded.\n%s\n", TTF_GetError());
		return;
	}

	SDL_Surface* load = TTF_RenderText_Solid(font, string, color);
	if (load == NULL)
	{
		printf("Render text failed.\n");
		return;
	}

	int width = load->w;
	int height = load->h;

	SDL_Texture* text = SDL_CreateTextureFromSurface(r, load);
	SDL_FreeSurface(load);
	if (text == NULL)
	{
		printf("Texture creation failed.");
		return;
	}

	SDL_RenderCopy(r, text, NULL, new SDL_Rect({ x, y, width, height }));

	SDL_DestroyTexture(text);
}

void printscr(SDL_Renderer* r, int x, int y, char* string, TTF_Font* font, TextAlignment align, SDL_Color color)
{
	if (r == NULL)
	{
		printf("No renderer loaded.");
		return;
	}

	if (font == NULL)
	{
		printf("No font loaded.");
		return;
	}

	char* next;
	char* str = strtok_s(string, "\n", &next);

	for (int i = 0; str != NULL; i++)
	{
		SDL_Surface* load = TTF_RenderText_Solid(font, str, color);
		if (load == NULL)
		{
			printf("Render text failed.\n");
			return;
		}

		int width = load->w;
		int height = load->h;

		SDL_Texture* text = SDL_CreateTextureFromSurface(r, load);
		SDL_FreeSurface(load);
		if (text == NULL)
		{
			printf("Texture creation failed.");
			return;
		}

		SDL_Rect* dest = NULL;

		if (align == TextAlignment::LEFT)
		{
			dest = new SDL_Rect({ x - width, y, width, height });
		}
		else if (align == TextAlignment::CENTER)
		{
			dest = new SDL_Rect({ x - width / 2, (y - height / 2) + height * i, width, height });
		}
		else if (align == TextAlignment::RIGHT)
		{
			dest = new SDL_Rect({ x, y, width, height });
		}

		SDL_RenderCopy(r, text, NULL, dest);

		SDL_DestroyTexture(text);
		delete dest;

		str = strtok_s(NULL, "\n", &next);
	}
}