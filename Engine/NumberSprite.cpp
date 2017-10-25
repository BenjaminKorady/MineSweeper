#include "NumberSprite.h"
#include <assert.h>

NumberSprite::NumberSprite(int value)
	:
	value(value)
{
	digits.push_back(Digit(value));
}

void NumberSprite::draw(Graphics & gfx, int x, int y)
{
	for (size_t i = 0; i < digits.size(); ++i) {
		digits[i].draw(gfx, x, y);
	}
}
