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

/**
	Constructs the game object

	@param wnd The main window in which the game runs
*/
Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	gameState(State::InMenu),
	menu(),
	timeDisplay(0)
{
}

/**
	Game loop
*/
void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

/**
	Logic update
*/
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
		}

	}	break;
	case State::InMenu: {
		if (menu.getSelectedOption() != Menu::Option::Name::None) {	// Menu option gets selected
			gameState = State::Playing;
			minefield = Minefield(menu); // Create minefield based on menu option
		}

	}  break;
	}
}

/**
	Loads the frame graphic into the memory
*/
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

/**
	Returns the game back into the menu
*/
void Game::restartGame()
{
	gameState = State::InMenu;
	menu.selectOption(Menu::Option::Name::None);
}

/**
	Returns true if at least one mine was revealed

	@param bool
*/
bool Game::gameHasStarted() const
{
	return minefield.getRevealedCounter() >= 1;
}

/**
	Manages all the user input (kseyboard and mouse)
*/
void Game::handleUserInput()
{
	
	// Loop while either the mouse or keyboard buffers are not empty
	while (!wnd.mouse.IsEmpty() || !wnd.kbd.KeyIsEmpty()) {
		Mouse::Event mouseEv;
		Keyboard::Event kbrdEv;
		// Store mouse event
		if (!wnd.mouse.IsEmpty()) {
			mouseEv = wnd.mouse.Read();
			lastMousePos = mouseEv.GetPos();
		}
		// Store keyboard event
		if (!wnd.kbd.KeyIsEmpty()) {
			const Keyboard::Event temp = wnd.kbd.ReadKey();
			if (temp.IsPress()) {
				kbrdEv = temp;	// Only store the keyboard event if a key was PRESSED (not released)
			}
		}

		
		switch (gameState) {
		case State::InMenu: {
			menu.highlightOption(menu.PointIsOverOption(lastMousePos));
			if (mouseEv.GetType() == Mouse::Event::Type::LPress) {
				menu.selectOption(menu.PointIsOverOption(lastMousePos));	// Selects the option over which the mouse is hovering
			}
		}
			break;
		case State::Playing: {
			if (minefield.tileExistsAtLocation(lastMousePos)) {
				if(mouseEv.GetType() == Mouse::Event::Type::LPress) {
					minefield.partiallyRevealTileAtLocation(lastMousePos); // Tile not revealed unless the user pressed
				}														   // and released mouse click on the same tile
				else if (mouseEv.GetType() == Mouse::Event::Type::LRelease) {
					if (minefield.tileAtLocationIsPartiallyRevealed(lastMousePos)) {
						if (minefield.getRevealedCounter() == 0) {	// If we are revealing the first tile
							gameStartTime = std::chrono::steady_clock::now();
						}
						minefield.revealTileAtLocation(lastMousePos);
					}
					else {	// Pressed on a tile but released on a different tile
						minefield.hidePartiallyRevealedTile();
					}
				}
				else if (
					   mouseEv.GetType() == Mouse::Event::Type::RLPress // Right + Left click at the same time (right first)
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
