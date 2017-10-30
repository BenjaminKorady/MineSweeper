/**
	Manages the game menu (Difficulty settings)

	@author Benjamin Korady
	@version 1.0 29/10/2017
*/

#pragma once
#include "Vei2.h"
#include "Graphics.h"

class Menu {
public:
	/**
		Each Option represents a difficulty and what values of the minefield it will set once selected (size, number of mines)
	*/
	class Option {
	public:
		enum class Name {
			Beginner,
			Intermediate,
			Expert,
			SIZE,
			None
		};

		Option() = default;
		Option(Name nameIn, Vei2 sizeIn, int minesIn, Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn);
		void draw(int x, int y, Graphics& gfx, bool glow);

	public:
		static constexpr int spacing = 54;	// Spacing between menu options in the selection

		Name name;
		Vei2 setsMinefieldSize;
		int setsMines;
		Vei2 spriteSize;
		Vei2 spriteGlowSize;
	};

public:
	Menu();

	void draw(Graphics& gfx);
	void highlightOption(Option::Name optionIn);
	void selectOption(Option::Name optionIn);

	Option::Name getSelectedOption() const;
	Option::Name PointIsOverOption(Vei2 pointIn) const;

public: 
	static constexpr int maxOptions = (int) Option::Name::SIZE;
	Option options[maxOptions];

private:
	const int getItemSizeX() const;
	const int getItemSizeY() const;
	Option::Name highlightedOption = Option::Name::None;
	Option::Name selectedOption = Option::Name::None;

};
