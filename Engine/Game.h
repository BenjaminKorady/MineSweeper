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

/**
	Main game logic

	@author Benjamin Korady
	@author Chili
	@version 1.0 27/10/2017
*/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Minefield.h"
#include "Menu.h"
#include <chrono>
#include "DigitalDisplay.h"

class Game
{
public:
	enum class InputType {
		Mouse,
		Keyboard
	};
	enum class State {
		Playing,
		Win,
		Loss,
		InMenu
	};
	
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete; 
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	void handleUserInput();
	void restartGame();
	bool gameHasStarted() const;
private:
	MainWindow& wnd;
	Graphics gfx;

	Menu menu;
	Minefield minefield;
	State gameState;
	Vei2 lastMousePos = { 0, 0 };
	DigitalDisplay timeDisplay;
	std::chrono::steady_clock::time_point gameStartTime;
	std::chrono::steady_clock::time_point timeNow;
	std::chrono::steady_clock::time_point gameEndTime;
	int elapsedTime = 0;
};