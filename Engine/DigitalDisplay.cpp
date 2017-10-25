#include "DigitalDisplay.h"
#include <algorithm>
DigitalDisplay::DigitalDisplay()
{
	*this = DigitalDisplay(0, defaultSize);
}

DigitalDisplay::DigitalDisplay(int value, int displayedDigits)
	:
	displayedDigits(displayedDigits),
	displayedNumber(NumberSprite(std::min(value, maxDisplayedValue), displayedDigits))
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

int DigitalDisplay::getWidth() const
{
	return displayedNumber.getWidth();
}

int DigitalDisplay::getHeight() const
{
	return digitHeight;
}

void DigitalDisplay::operator++()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() + 1, displayedDigits);
}

void DigitalDisplay::operator--()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() - 1, displayedDigits);
}
