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
			
	case State::InMenu: {
		if (menu.getSelectedOption() == Menu::Option::Name::None) {
			while (!wnd.mouse.IsEmpty()) {
				const Mouse::Event e = wnd.mouse.Read();
				lastMousePos = e.GetPos();

				if (e.GetType() == Mouse::Event::Type::LRelease) {
					menu.selectOption(menu.PointIsOverOption(lastMousePos));
				}
			}
			menu.highlightOption(menu.PointIsOverOption(lastMousePos));
		}
		else {
			gameState = State::Playing;
			int fieldWidth, fieldHeight, mineCount;

			switch (menu.getSelectedOption()) {
			case Menu::Option::Name::Beginner:	
				fieldWidth = menu.options[0].setsMinefieldSize.x;
				fieldHeight = menu.options[0].setsMinefieldSize.y;
				mineCount = menu.options[0].setsMines;
				break;
			case Menu::Option::Name::Intermediate: 	
				fieldWidth = menu.options[1].setsMinefieldSize.x;
				fieldHeight = menu.options[1].setsMinefieldSize.y;
				mineCount = menu.options[1].setsMines;
				break;
			case Menu::Option::Name::Expert:
				fieldWidth = menu.options[2].setsMinefieldSize.x;
				fieldHeight = menu.options[2].setsMinefieldSize.y;
				mineCount = menu.options[2].setsMines;
				break;
			}
			minefield = Minefield(fieldWidth, fieldHeight, mineCount);
		
		}

	}  break;

	}
	
}

void Game::restartGame()
{
	gameState = State::InMenu;
	menu.selectOption(Menu::Option::Name::None);
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
