#include "Menu.h"
#include "SpriteCodex.h"

Menu::Option::Option(Name nameIn, Vei2 sizeIn, int minesIn, Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn)
	:
	name(nameIn), setsMinefieldSize(sizeIn), setsMines(minesIn), spriteSize(spriteSizeIn), spriteGlowSize(spriteGlowSizeIn)
{
}

void Menu::Option::Draw(int x, int y, Graphics & gfx, bool glow)
{
	if (glow) {
		switch (name) {
		case Name::Beginner:
			SpriteCodex::drawBeginnerGlow(x, y, gfx); break;
		case Name::Intermediate:
			SpriteCodex::drawIntermediateGlow(x, y, gfx); break;
		case Name::Expert:
			SpriteCodex::drawExpertGlow(x, y, gfx); break;
		case Name::Custom:
			SpriteCodex::drawCustomGlow(x, y, gfx); break;
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
		case Name::Custom:
			SpriteCodex::drawCustom(x, y, gfx); break;
		}
	}
}


Menu::Menu()
{
	options[0] = Option(Option::Name::Beginner, Vei2(9, 9), 10, { 111, 27 }, { 116, 32 });
	options[1] = Option(Option::Name::Intermediate, Vei2(16, 16), 40, { 159, 21 }, { 164, 27 });
	options[2] = Option(Option::Name::Expert, Vei2(30, 16), 99, { 82, 27 }, { 86, 32 });
	options[3] = Option(Option::Name::Custom, Vei2(0, 0), 0, { 87, 20 }, { 93, 26 });
}

void Menu::Draw(Graphics & gfx)
{
	int offsetX = (Graphics::ScreenWidth) / 2;
	int offsetY = (Graphics::ScreenHeight - (getItemSizeY() * maxOptions)) / 2 + Option::spacing / 2;
	int x;
	int y;

	for (int i = 0; i < maxOptions; ++i) {
		x = offsetX - options[i].spriteSize.x / 2;
		y = offsetY + getItemSizeY()*i;
		options[i].Draw(x, y, gfx, highlightedOption == options[i].name);
	}

}

Menu::Option::Name Menu::getSelectedOption() const
{
	return selectedOption;
}

void Menu::highlightOption(Option::Name optionIn)
{
	highlightedOption = optionIn;
}

void Menu::selectOption(Option::Name optionIn)
{
	selectedOption = optionIn;
}

Menu::Option::Name Menu::PointIsOverOption(Vei2 pointIn) const
{
	RectI optionRectangle[maxOptions];

	int offsetX = (Graphics::ScreenWidth) / 2;
	int offsetY = (Graphics::ScreenHeight - (getItemSizeY() * maxOptions)) / 2 + Option::spacing / 2;
	int x;
	int y;


	for (int i = 0; i < maxOptions; ++i) {
		x = offsetX - options[i].spriteSize.x / 2;
		y = offsetY + getItemSizeY()*i;
		optionRectangle[i] = RectI(Vei2(x, y), options[i].spriteSize.x, options[i].spriteSize.y);

		if (optionRectangle[i].ContainsPoint(pointIn)) {
			return options[i].name;
		}
	}

	return Option::Name::None;

}

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

const int Menu::getItemSizeY() const
{

	int maxSizeY = 0;
	for (int i = 0; i < maxOptions; ++i) {
		if (options[i].spriteGlowSize.y > maxSizeY)
			maxSizeY = options[i].spriteGlowSize.y;
	}

	return maxSizeY + Option::spacing;
}