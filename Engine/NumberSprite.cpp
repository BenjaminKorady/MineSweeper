#include "NumberSprite.h"
#include <assert.h>
#include <algorithm>
NumberSprite::NumberSprite(int value)
	:
	value(value)
{	
	do {
		int digit = value % 10;
		digits.push_back(Digit(digit));
		value = value / 10;
	} while (value > 0);
	std::reverse(digits.begin(), digits.end());
}

NumberSprite::NumberSprite(int value, int size)
	:
	value(value)
{	
	int digitCounter = 0;
	do {
		int digit = value % 10;
		digits.push_back(Digit(digit));
		value = value / 10;
		++digitCounter;
	} while (value > 0);

	while (digitCounter < size) {
		digits.push_back(Digit(0));
		++digitCounter;
	}
	std::reverse(digits.begin(), digits.end());
}

void NumberSprite::draw(Graphics & gfx, int x, int y) const
{
	for (int i = 0; i < (int)digits.size(); ++i) {
		digits[i].draw(gfx, x+i*(Digit::spacing + Digit::width), y);
	}
}

int NumberSprite::getValue() const
{
	return value;
}
