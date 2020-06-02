#include "counter.h"

Counter::Counter(SDL_Renderer* r, int x, int y)
{
	currNum = {0, 0, width, 160};
	this->x = x - width / 2;
	this->y = y;

	this->r = r;
}

Counter::~Counter()
{
	
}

int Counter::getNum(void)
{
	return num;
}

void Counter::setNum(int num)
{
	this->num = num;
}

void Counter::increase(void)
{
	num++;
	currNum.x += width;
}

void Counter::render(void)
{
	currNum.x = (currNum.x >= width * 10) ? 0 : currNum.x;
	Object::render(x, y, &currNum);
}