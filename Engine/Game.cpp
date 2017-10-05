/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	gameState(State::InMenu),
	minefield(40),
	menu()
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	switch (gameState) {

	case State::Playing: {
			if (minefield.isExploded) {
				gameState = State::Loss;
			}
			else if (minefield.revealedAll()) {
				gameState = State::Win;
			}

			while (!wnd.mouse.IsEmpty()) {
				const Mouse::Event e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::LRelease) {
					if (minefield.tileExistsAtLocation(e.GetPos())) {
						minefield.revealTileAt(e.GetPos());
					}
				}
				else if (e.GetType() == Mouse::Event::Type::RRelease) {
					if (minefield.tileExistsAtLocation(e.GetPos())) {
						minefield.flagTileAt(e.GetPos());
					}
				}
				else if (e.GetType() == Mouse::Event::Type::MRelease) {
					if (minefield.tileExistsAtLocation(e.GetPos())) {
						minefield.revealOrFlagAt(e.GetPos());
					}
				}
			}
			while (!wnd.kbd.KeyIsEmpty()) {
				const Keyboard::Event e = wnd.kbd.ReadKey();
				if (e.IsPress()) {
					if (e.GetCode() == VK_SPACE) {
						const Vei2 pos = wnd.mouse.GetPos();
						if (minefield.tileExistsAtLocation(pos)) {
							minefield.revealOrFlagAt(pos);
						}
					}
				}
			}
		} break;
						
	case State::Loss: {
		while (!wnd.kbd.KeyIsEmpty()) {
			const Keyboard::Event e = wnd.kbd.ReadKey();
			if (e.IsRelease()) {
				if (e.GetCode() == VK_RETURN) {
					restartGame();
				}
			}
		}
	} break;
				
	case State::Win:{
		while (!wnd.kbd.KeyIsEmpty()) {
			const Keyboard::Event e = wnd.kbd.ReadKey();
			if (e.IsRelease()) {
				if (e.GetCode() == VK_RETURN) {
					restartGame();
				}
			}
		}

					  } break;
			
	case State::InMenu:{
		
	}  break;
		while (!wnd.mouse.IsEmpty()) {
			const Mouse::Event e = wnd.mouse.Read();
		}
	}
	
}

void Game::restartGame()
{
	gameState = State::Playing;
	minefield.restart();
}

void Game::ComposeFrame()
{
	if (gameState == State::InMenu) {
		menu.Draw(gfx);
	}
	else {
		minefield.draw(gfx);
	}

	switch (gameState) {

	case State::Win: 		
		SpriteCodex::drawGameWin(gfx); break;
	case State::Loss:
		SpriteCodex::drawGameLoss(gfx); break;
	}

}

Game::Menu::Option::Option(Name nameIn, Vei2 sizeIn, int minesIn, Vei2 spriteSizeIn, Vei2 spriteGlowSizeIn)
	:
	name(nameIn), setsMinefieldSize(sizeIn), setsMines(minesIn), spriteSize(spriteSizeIn), spriteGlowSize(spriteGlowSizeIn)
{
}

void Game::Menu::Option::Draw(int x, int y, Graphics & gfx, bool glow)
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


Game::Menu::Menu()
{
	options[0] = Option(Option::Name::Beginner, Vei2(9, 9), 10, { 111, 27 }, { 116, 32 });
	options[1] = Option(Option::Name::Intermediate, Vei2(16, 16), 40, { 159, 21 }, { 164, 27 });
	options[2] = Option(Option::Name::Expert, Vei2(16, 30), 99, { 82, 27 }, { 86, 32 });
	options[3] = Option(Option::Name::Custom, Vei2(0,0), 0, { 87, 20 }, { 93, 26 });
}

void Game::Menu::Draw(Graphics & gfx)
{
	int offsetX = (Graphics::ScreenWidth) / 2;
	int offsetY = (Graphics::ScreenHeight - (getItemSizeY() * maxOptions)) / 2 + Option::spacing/2;
	int x; 
	int y;

	for (int i = 0; i < maxOptions; ++i) {
		x = offsetX - options[i].spriteSize.x / 2;
		y = offsetY + getItemSizeY()*i;
		options[i].Draw(x, y, gfx, highlightedOption == options[i].name);
	}

}

Game::Menu::Option::Name Game::Menu::getSelectedOption() const
{
	return selectedOption;
}

void Game::Menu::highlightOption(Option::Name optionIn)
{
	highlightedOption = optionIn;
}

const int Game::Menu::getItemSizeX() const
{

	int maxSizeX = 0;
	int maxSizeY = 0;
	for (int i = 0; i < maxOptions; ++i) {
		if (options[i].spriteGlowSize.x > maxSizeX)
			maxSizeX = options[i].spriteGlowSize.x;
	}

	return maxSizeX;
}

const int Game::Menu::getItemSizeY() const
{

	int maxSizeY = 0;
	for (int i = 0; i < maxOptions; ++i) {
		if (options[i].spriteGlowSize.y > maxSizeY)
			maxSizeY = options[i].spriteGlowSize.y;
	}

	return maxSizeY + Option::spacing;
}