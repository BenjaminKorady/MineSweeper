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
			HandlePlayingMouseInput();
			HandlePlayingKeyboardInput();

		} break;
						
	case State::Loss: {
		HandleGameOverKeyboardInput();
		HandleGameOverMouseInput();
	} break;
				
	case State::Win: {
		HandleGameOverKeyboardInput();
		HandleGameOverMouseInput();
	} break;
			
	case State::InMenu: {
		if (menu.getSelectedOption() == Menu::Option::Name::None) {
			HandleInMenuMouseInput();
		}
		else {
			gameState = State::Playing;
			minefield = Minefield(menu); // Create minefield based on menu option
		}

	}  break;

	}
	
}

void Game::ComposeFrame()
{
	if (gameState == State::InMenu) {
		menu.Draw(gfx);
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

void Game::HandlePlayingKeyboardInput()
{
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
}

void Game::HandleGameOverMouseInput()
{
	while (!wnd.mouse.IsEmpty()) {
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LRelease) {
			if (wnd.mouse.IsInWindow()) {
				restartGame();
			}
		}
	}
}

void Game::HandleGameOverKeyboardInput()
{
	while (!wnd.kbd.KeyIsEmpty()) {
		const Keyboard::Event e = wnd.kbd.ReadKey();
		if (e.IsRelease()) {
			if (e.GetCode() == VK_RETURN) {
				restartGame();
			}
		}
	}

}

void Game::HandleInMenuMouseInput()
{
	while (!wnd.mouse.IsEmpty()) {
		const Mouse::Event e = wnd.mouse.Read();
		lastMousePos = e.GetPos();

		if (e.GetType() == Mouse::Event::Type::LRelease) {
			menu.selectOption(menu.PointIsOverOption(lastMousePos));
		}
	}
	menu.highlightOption(menu.PointIsOverOption(lastMousePos));
}

bool Game::gameHasStarted() const
{
	return minefield.getRevealedCounter() >= 1;
}

void Game::HandlePlayingMouseInput()
{
	while (!wnd.mouse.IsEmpty()) {
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LRelease) {
			if (minefield.tileExistsAtLocation(e.GetPos())) {
				if (minefield.getRevealedCounter() == 0) {
					gameStartTime = std::chrono::steady_clock::now();
				}
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
}

