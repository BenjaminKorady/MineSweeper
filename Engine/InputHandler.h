#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "Minefield.h"
#include "Game.h"

namespace InputHandler {
	enum class Type {
		Keyboard,
		Mouse
	};
	enum class GameState {
		Playing,
		GameOver,
		InMenu
	};
	class Input {
	public:
		Input(Type type, GameState state)
			:
			type(type),
			state(state)
		{}
		void operator()(Mouse& mouse, Minefield& minefield) {
			if(state == GameState::Playing) {
				while (!mouse.IsEmpty()) {
					const Mouse::Event e = mouse.Read();
					if (e.GetType() == Mouse::Event::Type::LPress && minefield.tileExistsAtLocation(e.GetPos())) {
						minefield.partiallyRevealTileAt(e.GetPos());
					}
					if (e.GetType() == Mouse::Event::Type::LRelease) {
						if (minefield.tileExistsAtLocation(e.GetPos()) && minefield.tileAtLocationIsPartiallyRevealed(e.GetPos())) {
							if (minefield.getRevealedCounter() == 0) {
							}
							minefield.revealTileAt(e.GetPos());
						}
						else {
							minefield.hidePartiallyRevealedTile();
						}
					}

					else if (e.GetType() == Mouse::Event::Type::RPress) {
						if (minefield.tileExistsAtLocation(e.GetPos())) {
							minefield.flagTileAt(e.GetPos());
						}
					}
					else if ((e.GetType() == Mouse::Event::Type::MPress) || (mouse.LeftIsPressed() && mouse.RightIsPressed())) {
						if (minefield.tileExistsAtLocation(e.GetPos())) {
							minefield.revealOrFlagAt(e.GetPos());
						}
					}
				}
			}
			else if (state == GameState::GameOver) {

			}
		}
	private:
		Type type;
		GameState state;
	};
}