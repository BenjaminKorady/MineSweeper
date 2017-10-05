/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Minefield.h"

class Game
{
public:
	enum class State {
		Playing,
		Win,
		Loss,
		InMenu
	};

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
			Option(Name nameIn, Vei2 sizeIn, int minesIn,  Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn);

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
	private:
		const int Game::Menu::getItemSizeX() const;
		const int Game::Menu::getItemSizeY() const;
		static constexpr int maxOptions = 4;
		Option options[maxOptions];
		Option::Name highlightedOption = Option::Name::None;
		Option::Name selectedOption = Option::Name::None;
	};

public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete; 
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
	void restartGame();
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	Menu menu;
	Minefield minefield;
	State gameState;
	Vei2 lastMousePos;
};