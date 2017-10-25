#pragma once
#include "Graphics.h"
#include <assert.h>
#include <vector>

class NumberSprite {
private:
	class Digit {
		enum class Portion {
			top,
			topLeft,
			topRight,
			center,
			bottomLeft,
			bottomRight,
			bottom,
			SIZE
		};

	public:
		Digit(int value):
			value(value)
		{
			assert(value <= 9 && value >= 0);
			switch (value) {
			case 0: portions = 0b1110111; break;
			case 1: portions = 0b0010010; break;
			case 2: portions = 0b1011101; break;
			case 3: portions = 0b1011011; break;
			case 4: portions = 0b0111010; break;
			case 5: portions = 0b1101011; break;
			case 6: portions = 0b1101111; break;
			case 7: portions = 0b1010010; break;
			case 8: portions = 0b1111111; break;
			case 9: portions = 0b1111011; break;
			}
			assert(portions < 0b10000000);
		}		
		void draw(Graphics& gfx, int x, int y) {
			unsigned char currentPortionOn = 0b01000000;
			for (int p = 0; p < (int)Portion::SIZE; ++p) {
				drawPortion(gfx, Portion(p), bool(portions & currentPortionOn), x, y);
				currentPortionOn = currentPortionOn >> 1;
			}
		}

	private:

	private:
		void drawPortion(Graphics& gfx, Portion portion, bool isOn, int x, int y) {
			Color c = isOn ? on : off;
			switch (portion) {
			case Portion::top: {
				gfx.PutPixel(x + 1, y, c);
				gfx.PutPixel(x + 3, y, c);
				gfx.PutPixel(x + 5, y, c);
				gfx.PutPixel(x + 7, y, c);
				gfx.PutPixel(x + 9, y, c);
				gfx.PutPixel(x + 2, y + 1, c);
				gfx.PutPixel(x + 4, y + 1, c);
				gfx.PutPixel(x + 6, y + 1, c);
				gfx.PutPixel(x + 8, y + 1, c);
				gfx.PutPixel(x + 3, y + 2, c);
				gfx.PutPixel(x + 5, y + 2, c);
				gfx.PutPixel(x + 7, y + 2, c);
				if (isOn) {
					gfx.PutPixel(x + 2, y, c);
					gfx.PutPixel(x + 4, y, c);
					gfx.PutPixel(x + 6, y, c);
					gfx.PutPixel(x + 8, y, c);
					gfx.PutPixel(x + 3, y + 1, c);
					gfx.PutPixel(x + 5, y + 1, c);
					gfx.PutPixel(x + 7, y + 1, c);
					gfx.PutPixel(x + 4, y + 2, c);
					gfx.PutPixel(x + 6, y + 2, c);
				}
			}
			case Portion::topLeft: {
				gfx.PutPixel(x    , y + 1, c);
				gfx.PutPixel(x + 1, y + 2, c);
				gfx.PutPixel(x    , y + 3, c);
				gfx.PutPixel(x + 2, y + 3, c);
				gfx.PutPixel(x + 1, y + 4, c);
				gfx.PutPixel(x    , y + 5, c);
				gfx.PutPixel(x + 2, y + 5, c);
				gfx.PutPixel(x + 1, y + 6, c);
				gfx.PutPixel(x    , y + 7, c);
				gfx.PutPixel(x + 2, y + 7, c);
				gfx.PutPixel(x + 1, y + 8, c);
				gfx.PutPixel(x    , y + 9, c);
				if (isOn) {
					gfx.PutPixel(x, y + 2, c);
					gfx.PutPixel(x + 1, y + 3, c);
					gfx.PutPixel(x, y + 4, c);
					gfx.PutPixel(x + 2, y + 4, c);
					gfx.PutPixel(x + 1, y + 5, c);
					gfx.PutPixel(x, y + 6, c);
					gfx.PutPixel(x + 2, y + 6, c);
					gfx.PutPixel(x + 1, y + 7, c);
					gfx.PutPixel(x, y + 8, c);
				}
			}
			default: {}

			}
		}

		unsigned char portions;
		int value;

		static constexpr Color background = Colors::Black;
		static constexpr Color on = Colors::Red;
		static constexpr Color off = Color(123, 0, 0);
	};

public:
	NumberSprite(int value);
	void draw(Graphics& gfx, int x, int y);
private:
	int value;
	std::vector<Digit> digits;
};