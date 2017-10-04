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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	gameState( State::Playing ),
	minefield(40)
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
	if (gameState == State::Playing) {
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
		}
	}

	else if (gameState == State::Loss || gameState == State::Win) {
		while (!wnd.kbd.KeyIsEmpty()) {
			const Keyboard::Event e = wnd.kbd.ReadKey();
			if (e.IsRelease()) {
				if (e.GetCode() == VK_RETURN) {
					restartGame();
				}
			}
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
	minefield.draw(gfx);

	if (gameState == State::Win) {
		SpriteCodex::drawGameWin(gfx);
	}
	else if (gameState == State::Loss) {
		SpriteCodex::drawGameLoss(gfx);
	}
}
