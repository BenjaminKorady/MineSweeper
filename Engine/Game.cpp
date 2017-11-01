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
#include "DigitalDisplay.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	gameState(State::InMenu),
	menu(),
	timeDisplay(0)
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
	handleUserInput();
	switch (gameState) {
	case State::Playing: {
		if (minefield.isExploded) {
			gameState = State::Loss;
		}
		else if (minefield.revealedAll()) {
			gameState = State::Win;
			gameEndTime = std::chrono::steady_clock::now();
		}
		else { // Game is running
			timeNow = std::chrono::steady_clock::now();
			if (gameHasStarted()) {
				elapsedTime = (int)std::chrono::duration_cast<std::chrono::seconds>(timeNow - gameStartTime).count();
				timeDisplay = DigitalDisplay(elapsedTime);
			}			
			// else elaspedTime = 0;
		}

		} break;
	case State::InMenu: {
		if (menu.getSelectedOption() != Menu::Option::Name::None) {
			gameState = State::Playing;
			minefield = Minefield(menu); // Create minefield based on menu option
		}

	}  break;

	}
	
}

void Game::ComposeFrame()
{
	if (gameState == State::InMenu) {
		menu.draw(gfx);
	}
	else {
		minefield.draw(gfx);
		int x = (Graphics::ScreenWidth + minefield.getWidth()) / 2 - timeDisplay.getWidth();
		int y = (Graphics::ScreenHeight - minefield.getHeight()) / 2 - timeDisplay.getHeight() - Minefield::displayOffset;
		timeDisplay.draw(gfx, x, y);
	}

	switch (gameState) {

	case State::Win:
		SpriteCodex::drawGameWin(gfx); break;
	case State::Loss:
		SpriteCodex::drawGameLoss(gfx); break;
	}
}

void Game::restartGame()
{
	gameState = State::InMenu;
	menu.selectOption(Menu::Option::Name::None);
}

bool Game::gameHasStarted() const
{
	return minefield.getRevealedCounter() >= 1;
}

void Game::handleUserInput()
{
	// Mouse input
	while (!wnd.mouse.IsEmpty() || !wnd.kbd.KeyIsEmpty()) {
		Mouse::Event mouseEv;
		Keyboard::Event kbrdEv;
		if (!wnd.mouse.IsEmpty()) {
			mouseEv = wnd.mouse.Read();
			lastMousePos = mouseEv.GetPos();
		}
		if (!wnd.kbd.KeyIsEmpty()) {
			const Keyboard::Event temp = wnd.kbd.ReadKey();
			if (temp.IsPress()) {
				kbrdEv = temp;
			}
		}

		switch (gameState) {
		case State::InMenu: {
			menu.highlightOption(menu.PointIsOverOption(lastMousePos));
			if (mouseEv.GetType() == Mouse::Event::Type::LPress) {
				menu.selectOption(menu.PointIsOverOption(lastMousePos));
			}
		}
			break;
		case State::Playing: {
			if (minefield.tileExistsAtLocation(lastMousePos)) {
				if(mouseEv.GetType() == Mouse::Event::Type::LPress) {
					minefield.partiallyRevealTileAtLocation(lastMousePos);
				}
				else if (mouseEv.GetType() == Mouse::Event::Type::LRelease) {
					if (minefield.tileAtLocationIsPartiallyRevealed(lastMousePos)) {
						if (minefield.getRevealedCounter() == 0) {
							gameStartTime = std::chrono::steady_clock::now();
						}
						minefield.revealTileAtLocation(lastMousePos);
					}
					else {
						minefield.hidePartiallyRevealedTile();
					}
				}
				else if (
					   mouseEv.GetType() == Mouse::Event::Type::RLPress 
					|| mouseEv.GetType() == Mouse::Event::Type::MPress 
					|| kbrdEv.GetCode() == VK_SPACE) 
				{
					minefield.revealSurroundingTilesOrFlagTileAtLocation(lastMousePos);
				}
				else if (mouseEv.GetType() == Mouse::Event::Type::RPress) {
					minefield.toggleTileFlagAtLocation(mouseEv.GetPos());
				}
			}
		}
			break;
		case State::Loss:
		case State::Win: {
			if (mouseEv.GetType() == Mouse::Event::Type::LPress
				|| kbrdEv.GetCode() == VK_SPACE
				|| kbrdEv.GetCode() == VK_RETURN) 
			{
				restartGame();
			}
		}
			break;
		}
	}
} 
