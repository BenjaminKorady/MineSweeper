#include "NumberSprite.h"
#include <algorithm>

NumberSprite::NumberSprite(int value, int size)
	:
	value(value)
{	
	int digitCounter = 0;

	// Push each digit into digits vector
	do {
		int digit = abs(value % 10);
		digits.push_back(Digit(digit));
		value = value / 10;
		++digitCounter;
	} while (value != 0);
	
	// Fill the rest of the display with 0s if all digits have been pushed
	// (int)(this->value < 0) means "push one 0 less if the number is negative" to reserve a slot for '-' symbol
	while (digitCounter < size - (int)(this->value < 0)) { 
		digits.push_back(Digit(0));
		++digitCounter;
	}

	if (this->value < 0) {
		digits.push_back(Digit(-1));	// Digit class takes (-1) as negation symbol
	}

	// Digits were pushed in reverse order, now just reverse the vector to get this in the correct order
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

int NumberSprite::getWidth() const
{
	return (Digit::width + Digit::spacing) * (int) digits.size() - Digit::spacing ;
}
