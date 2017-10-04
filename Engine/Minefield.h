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
		void draw(Graphics& gfx, bool isExploded) const;
		
		Vei2 getPosition() const;
		void setPosition(Vei2 posIn);
		void setAdjacentMines(int in);
		int getAdjacentMines() const;
		void spawnMine();
		void reveal();
		void flag();
		State getState() const;
		bool hasMine() const;
		void restart();


		static constexpr int width = 16;
		static constexpr int height	 = 16;

	private:

		Vei2 position;
		int adjacentMines;
		State state = State::Hidden;
		bool mine = false;

	};

public:
	Minefield(int nMines);
    
	void draw(Graphics& gfx);
	void revealTileAt(Vei2& globalLocation);
	void revealRecursively(Tile& tileIn);
	bool revealSurrounding(Tile& tileIn);
	void flagTileAt(Vei2& globalLocation);
	const Tile& tileAt(const Vei2& tileLocation) const;
	Tile& tileAt(const Vei2& tileLocation);
	bool revealedAll() const;
	bool isExploded = false;
	bool tileExistsAtLocation(const Vei2& globalLocation) const;
	void restart();

	static constexpr int width = 16;
	static constexpr int height = 16;

private:
	Tile field[width*height];
	Vei2 position = Vei2(0, 0);
	int nMines;
	Vei2 getTileLocation(const Vei2& globalPosition) const;
	int getAdjacentMines(const Tile& tileIn) const;
	int revealedCounter = 0;
	RectI rectangle;



};