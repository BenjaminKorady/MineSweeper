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

Game::Menu::Option::Option(Name nameIn, Vei2 sizeIn, int minesIn)
	:
	name(nameIn), setsMinefieldSize(sizeIn), setsMines(minesIn)
{
}

Game::Menu::Option::Option(Name nameIn)
	:
	name(nameIn)
{
}

Game::Menu::Option::Option()
{
	name = Name::None;
}

Game::Menu::Menu()
{
	options[0] = Option(Option::Name::Beginner, Vei2(9,9), 10);
	options[1] = Option(Option::Name::Intermediate, Vei2(16, 16), 40);
	options[2] = Option(Option::Name::Expert, Vei2(16, 30), 99);
//	options[4] = Option(Option::Name::Custom);
}

void Game::Menu::Draw(Graphics & gfx)
{
	const int menuOptions = 4;
	const int maxSpriteHeight = 55;	// only on glow
	const Vei2 center = { Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2 };

	const Vei2 beginnerSize = { 111, 27 };
	const Vei2 intermediateSize = { 160, 20 };
	const Vei2 expertSize = { 83, 25 };
	const Vei2 customSize = { 89, 19 };
	const Vei2 beginnerGlowSize = { 116, 32 };

	switch (highlightedOption) {
	case Option::Name::Beginner:
		SpriteCodex::drawBeginnerGlow(center.x - beginnerGlowSize.x / 2, center.y - 2 * maxSpriteHeight, gfx);
		SpriteCodex::drawIntermediate(center.x - intermediateSize.x / 2, center.y - maxSpriteHeight, gfx);
		SpriteCodex::drawExpert(center.x - expertSize.x / 2, center.y, gfx);
		SpriteCodex::drawCustom(center.x - customSize.x / 2, center.y + maxSpriteHeight, gfx);
		break;
	case Option::Name::Intermediate:
		SpriteCodex::drawBeginner(center.x - beginnerSize.x / 2, center.y - 2 * maxSpriteHeight, gfx);
		SpriteCodex::drawIntermediateGlow(center.x - intermediateSize.x / 2, center.y - maxSpriteHeight, gfx);
		SpriteCodex::drawExpert(center.x - expertSize.x / 2, center.y, gfx);
		SpriteCodex::drawCustom(center.x - customSize.x / 2, center.y + maxSpriteHeight, gfx);
		break;
	case Option::Name::Expert:
		SpriteCodex::drawBeginner(center.x - beginnerSize.x / 2, center.y - 2 * maxSpriteHeight, gfx);
		SpriteCodex::drawIntermediate(center.x - intermediateSize.x / 2, center.y - maxSpriteHeight, gfx);
		SpriteCodex::drawExpertGlow(center.x - expertSize.x / 2, center.y, gfx);
		SpriteCodex::drawCustom(center.x - customSize.x / 2, center.y + maxSpriteHeight, gfx);
		break;
	case Option::Name::Custom:
		SpriteCodex::drawBeginner(center.x - beginnerSize.x / 2, center.y - 2 * maxSpriteHeight, gfx);
		SpriteCodex::drawIntermediate(center.x - intermediateSize.x / 2, center.y - maxSpriteHeight, gfx);
		SpriteCodex::drawExpert(center.x - expertSize.x / 2, center.y, gfx);
		SpriteCodex::drawCustomGlow(center.x - customSize.x / 2, center.y + maxSpriteHeight, gfx);
		break;
	case Option::Name::None:
		SpriteCodex::drawBeginner(center.x - beginnerSize.x / 2, center.y - 2 * maxSpriteHeight, gfx);
		SpriteCodex::drawIntermediate(center.x - intermediateSize.x / 2, center.y - maxSpriteHeight, gfx);
		SpriteCodex::drawExpert(center.x - expertSize.x / 2, center.y, gfx);
		SpriteCodex::drawCustom(center.x - customSize.x / 2, center.y + maxSpriteHeight, gfx);
		break;
	}

}

Game::Menu::Option::Name Game::Menu::getSelectedOption()
{
	return selectedOption;
}

void Game::Menu::highlightOption(Option::Name optionIn)
{
	highlightedOption = optionIn;
}
