#include "Menu.h"
#include "SpriteCodex.h"

/**
	Constructs an Option object with input parameters

	@param nameIn
	@param sizeIn
	@param minesIn
	@param spriteSizeIn
	@param spriteGlowSizeIn
*/
Menu::Option::Option(Name nameIn, Vei2 sizeIn, int minesIn, Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn)
	:
	name(nameIn), setsMinefieldSize(sizeIn), setsMines(minesIn), spriteSize(spriteSizeIn), spriteGlowSize(spriteGlowSizeIn)
{
}

/**
	Draws a menu option to the screen

	@param x X-coordinate of the position where the option is to be drawn
	@param y Y-coordinate of the position where the option is to be drawn
	@param gfx Graphics processor
	@param glow Set true if the drawn menu option is to be glowing
*/
void Menu::Option::draw(int x, int y, Graphics & gfx, bool glow)
{
	if (glow) {
		switch (name) {
		case Name::Beginner:
			SpriteCodex::drawBeginnerGlow(x, y, gfx); break;
		case Name::Intermediate:
			SpriteCodex::drawIntermediateGlow(x, y, gfx); break;
		case Name::Expert:
			SpriteCodex::drawExpertGlow(x, y, gfx); break;
		}
	}
	else {
		switch (name) {
		case Name::Beginner:
			SpriteCodex::drawBeginner(x, y, gfx); break;
		case Name::Intermediate:
			SpriteCodex::drawIntermediate(x, y, gfx); break;
		case Name::Expert:
			SpriteCodex::drawExpert(x, y, gfx); break;
		}
	}
}

/**
	Constructs the Menu and automatically assigns the constant values of the minefield and sprite sizes
*/
Menu::Menu()
{						// Name					 |  Minefield size	|	Mines	|	Text size	 |	Glowing text size  |
	options[0] = Option(Option::Name::Beginner,		Vei2(9, 9),			10,			{ 111, 27 },	{ 116, 32 }		);
	options[1] = Option(Option::Name::Intermediate, Vei2(16, 16),		40,			{ 159, 21 },	{ 164, 27 }		);
	options[2] = Option(Option::Name::Expert,		Vei2(30, 16),		99,			{ 82, 27 },		{ 86, 32 }		);
}

/**
	Draws the menu to the screen

	@param gfx Graphics processor
*/
void Menu::draw(Graphics & gfx)
{
	// Offset from the edges of the screen, used to draw the menu to the center
	int offsetX = (Graphics::ScreenWidth) / 2;
	int offsetY = (Graphics::ScreenHeight - (getItemSizeY() * maxOptions)) / 2 + Option::spacing / 2;
	int x;
	int y;

	for (int i = 0; i < maxOptions; ++i) {
		x = offsetX - options[i].spriteSize.x / 2;
		y = offsetY + getItemSizeY()*i;
		options[i].draw(x, y, gfx, highlightedOption == options[i].name);
	}

}

/**
	Returns the currently selected option
	
	@return selectedOption
*/
Menu::Option::Name Menu::getSelectedOption() const
{
	return selectedOption;
}

/**
	Sets the input option to be highlighted

	@param optionIn
*/
void Menu::highlightOption(Option::Name optionIn)
{
	highlightedOption = optionIn;
}

/**
	Sets the input option to be selected

	@param optionIn
*/
void Menu::selectOption(Option::Name optionIn)
{
	selectedOption = optionIn;
}

/**
	Returns option with which the input point overlaps

	@return Menuu::Option::Name Name of the option with which the point overlapss
*/
Menu::Option::Name Menu::PointIsOverOption(Vei2 pointIn) const
{
	RectI optionRectangle[maxOptions];

	int offsetX = (Graphics::ScreenWidth) / 2;
	int offsetY = (Graphics::ScreenHeight - (getItemSizeY() * maxOptions)) / 2 + Option::spacing / 2;
	int x;
	int y;

	// Constructs a rectangle object for each option
	for (int i = 0; i < maxOptions; ++i) {
		x = offsetX - options[i].spriteSize.x / 2;
		y = offsetY + getItemSizeY()*i;
		optionRectangle[i] = RectI(Vei2(x, y), options[i].spriteSize.x, options[i].spriteSize.y);

		// Checks if the rectangle contains a point
		if (optionRectangle[i].ContainsPoint(pointIn)) {
			return options[i].name;
		}
	}

	return Option::Name::None;
}

/**
	Returns the width of the largest menu option

	@return maxSizeX
*/
const int Menu::getItemSizeX() const
{

	int maxSizeX = 0;
	int maxSizeY = 0;
	for (int i = 0; i < maxOptions; ++i) {
		if (options[i].spriteGlowSize.x > maxSizeX)
			maxSizeX = options[i].spriteGlowSize.x;
	}

	return maxSizeX;
}

/**
	Returns the height of the largest menu option

	@return maxSizeY
*/
const int Menu::getItemSizeY() const
{

	int maxSizeY = 0;
	for (int i = 0; i < maxOptions; ++i) {
		if (options[i].spriteGlowSize.y > maxSizeY)
			maxSizeY = options[i].spriteGlowSize.y;
	}

	return maxSizeY + Option::spacing;
}