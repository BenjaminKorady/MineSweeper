/**
	Manages the digital display showing mines left / timer

	@author Benjamin Korady
	@version 1.0 27/10/2017
*/

#pragma once
#include "NumberSprite.h"
#include <chrono>
#include "Graphics.h"

class DigitalDisplay {
public:
	DigitalDisplay();
	DigitalDisplay(int value, int displayedDigits = defaultSize);

	void operator++();
	void operator--();

	void draw(Graphics& gfx, int x, int y) const;
	int getValue() const;
	int getWidth() const;
	static int getHeight();


private:
	static constexpr int defaultSize = 3;
	static constexpr int maxDisplayedValue = 999;

private:
	int displayedDigits;
	NumberSprite displayedNumber;
};