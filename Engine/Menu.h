#pragma once
#include "Vei2.h"
#include "Graphics.h"

class Menu {
public:
	class Option {
	public:
		enum class Name {
			Beginner,
			Intermediate,
			Expert,
			Custom,
			None
		};

	public:
		Option() = default;
		Option(Name nameIn, Vei2 sizeIn, int minesIn, Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn);
		void Draw(int x, int y, Graphics& gfx, bool glow);
		static constexpr int spacing = 54;

	public:
		Name name;
		Vei2 setsMinefieldSize;
		Vei2 spriteSize;
		Vei2 spriteGlowSize;
		int setsMines;

	};

public:
	Menu();
	void Draw(Graphics& gfx);
	Option::Name getSelectedOption() const;
	void highlightOption(Option::Name optionIn);
	void selectOption(Option::Name optionIn);
	Option::Name PointIsOverOption(Vei2 pointIn) const;
	static constexpr int maxOptions = 4;
	Option options[maxOptions];
private:
	const int getItemSizeX() const;
	const int getItemSizeY() const;
	Option::Name highlightedOption = Option::Name::None;
	Option::Name selectedOption = Option::Name::None;
};
