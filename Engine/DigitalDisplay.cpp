#include "DigitalDisplay.h"

DigitalDisplay::DigitalDisplay()
{
	*this = DigitalDisplay(0, defaultSize);
}

DigitalDisplay::DigitalDisplay(int start, int displayedDigits)
	:
	displayedDigits(displayedDigits),
	displayedNumber(NumberSprite(start, displayedDigits))
{
}

void DigitalDisplay::draw(Graphics & gfx, int x, int y)
{
	displayedNumber.draw(gfx, x, y);
}

int DigitalDisplay::getValue() const
{
	return displayedNumber.getValue();
}

void DigitalDisplay::operator++()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() + 1, displayedDigits);
}

void DigitalDisplay::operator--()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() - 1, displayedDigits);
}
