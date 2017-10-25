#pragma once
#include "NumberSprite.h"
#include <chrono>
#include "Graphics.h"
class DigitalDisplay {
public:
	DigitalDisplay();
	DigitalDisplay(int start, int displayedDigits = defaultSize);

	void draw(Graphics& gfx, int x, int y);
	int getValue() const;
	void operator++();
	void operator--();

	static const int digitHeight = 21;
private:
	static const int defaultSize = 3;
private:
	int displayedDigits;
	NumberSprite displayedNumber;
};