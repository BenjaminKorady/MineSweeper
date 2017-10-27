#include "DigitalDisplay.h"
#include <algorithm>

/**
	Constructs a DigitalDisplay object with 0 as the default value
*/
DigitalDisplay::DigitalDisplay()
{
	*this = DigitalDisplay(0, defaultSize);
}

/**
	Constructs a DigitDisplay object

	@param value Value to be shown on the display
	@param displayedDigit The amount of digits to be displayed (Default is 3)
*/
DigitalDisplay::DigitalDisplay(int value, int displayedDigits)
	:
	displayedDigits(displayedDigits),
	displayedNumber(NumberSprite(std::min(value, maxDisplayedValue) /* Caps value at 999 */, displayedDigits))
{
}

/**
	Draws the Digital Display to the screen

	@param gfx Graphics processor
	@param x x-value of the position where the display is to be drawn
	@param y y-value of the position where the display is to be drawn
*/
void DigitalDisplay::draw(Graphics & gfx, int x, int y) const
{
	displayedNumber.draw(gfx, x, y);
}

/**
	Returns the value of the number that is currently shown on the display

	@return value
*/
int DigitalDisplay::getValue() const
{
	return displayedNumber.getValue();
}

/**
	Returns the width of the display (in pixels)

	@return width
*/
int DigitalDisplay::getWidth() const
{
	return displayedNumber.getWidth();
}

/**
	Returns the height of the display (in pixels)

	@return height
*/
int DigitalDisplay::getHeight()
{
	return NumberSprite::getHeight();
}

/**
	Increments the number drawn on the display
*/
void DigitalDisplay::operator++()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() + 1, displayedDigits);
}

/**
	Decrements the number drawn on the display
*/
void DigitalDisplay::operator--()
{
	displayedNumber = NumberSprite(displayedNumber.getValue() - 1, displayedDigits);
}
