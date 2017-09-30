#pragma once

#include "Graphics.h"
#include "Vei2.h"
class Minefield {
private:
	class Tile {
	public:
		enum class State {
			Hidden = 0,
			Revealed = 1,
			Flagged = 2
		};

		Tile();
		Tile(Vei2 posIn);
		void draw(Graphics& gfx) const;
		
		void setPosition(Vei2 posIn);
		void spawnMine();
		void reveal();
		void flag();
		bool hasMine() const;

		static constexpr int width = 16;
		static constexpr int height	 = 16;
	private:

		Vei2 position;
		State state = State::Hidden;
		bool mine = false;
	};

public:
	Minefield(int nMines);
    
	void draw(Graphics& gfx);
	void revealTileAt(Vei2& clickLocation);
	void flagTileAt(Vei2& clickLocation);
	static constexpr int width = 20;
	static constexpr int height = 15;

private:
	Tile field[width*height];
	Vei2 position = Vei2(0, 0);
	int nMines;
	Vei2 getTileLocation(const Vei2& clickLocation) const;

};