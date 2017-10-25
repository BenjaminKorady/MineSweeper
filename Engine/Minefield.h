#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "Menu.h"
#include "DigitalDisplay.h"

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
		bool flag();
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
	Minefield() = default;
	Minefield(int widthIn, int heightIn, int nMinesIn);
	Minefield(const Menu& menu);

	void draw(Graphics& gfx);
	void revealTileAt(Vei2& globalLocation);
	void revealRecursively(Tile& tileIn);
	bool revealSurrounding(Tile& tileIn);
	void revealOrFlagAt(const Vei2& globalLocation);
	void flagTileAt(const Vei2& globalLocation);
	const Tile& tileAt(const Vei2& tileLocation) const;
	Tile& tileAt(const Vei2& tileLocation);
	void restart();


	bool revealedAll() const;
	bool tileExistsAtLocation(const Vei2& globalLocation) const;
	int getFlaggedCounter() const;
	int getMineCounter() const;
	int getRevealedCounter() const;
	int getWidth() const;
	int getHeight() const;

	bool isExploded = false;
	static constexpr int displayOffset = 5;

private:

private:
	Vei2 getTileLocation(const Vei2& globalPosition) const;
	int getAdjacentMines(const Tile& tileIn) const;
	void updateDisplay();

	Tile* field = nullptr;
	int width;
	int height;
	Vei2 position = Vei2(0, 0);
	int nMines;
	int revealedCounter = 0;
	int flaggedCounter = 0;
	RectI rectangle;
	DigitalDisplay minesLeftDisplay;


};