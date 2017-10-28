/**
	Manages the minefield 

	@author Benjamin Korady
	@version 1.0 27/10/2017
*/

#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "Menu.h"
#include "DigitalDisplay.h"

class Minefield {
private:
	/**
		Manages each Tile of the minefield
	*/
	class Tile {
	public:
		enum class State {
			Hidden,
			PartiallyRevealed,
			Revealed,
			Flagged,
		};

		Tile() = default;
		void draw(Graphics& gfx, bool isExploded) const;
		
		void setPosition(Vei2 posIn);
		void setAdjacentMines(int in);
		void spawnMine();
		void clearMine();
		void reveal();
		void partiallyReveal();
		void hide();
		bool flag();
		void restart();

		State getState() const;
		bool hasMine() const;
		int getAdjacentMines() const;
		Vei2 getPosition() const;

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

	void generateMines(Tile& clickedTile);
	void clearMines();
	void draw(Graphics& gfx);
	void partiallyRevealTile(Tile& tileIn);
	void partiallyRevealTileAt(Vei2& globalLocation);
	void revealTileAt(Vei2& globalLocation);
	void revealRecursively(Tile& tileIn);
	bool revealSurrounding(Tile& tileIn);
	void revealOrFlagAt(const Vei2& globalLocation);
	void flagTileAt(const Vei2& globalLocation);
	const Tile& tileAt(const Vei2& tileLocation) const;
	Tile& tileAt(const Vei2& tileLocation);
	void hidePartiallyRevealedTile();
	void restart();


	bool revealedAll() const;
	bool tileExistsAtLocation(const Vei2& globalLocation) const;
	bool tileAtLocationIsPartiallyRevealed(const Vei2& globalLocation) const;
	int getFlaggedCounter() const;
	int getMineCounter() const;
	int getRevealedCounter() const;
	int getWidth() const;
	int getHeight() const;

	bool isExploded = false;
	static constexpr int displayOffset = 5;

private:
	bool minesAreGenerated = false;
private:
	Vei2 getTileLocation(const Vei2& globalPosition) const;
	int getAdjacentMines(const Tile& tileIn) const;
	void updateDisplay();

	Tile* field = nullptr;
	Tile* partiallyRevealedTilePtr = nullptr;
	int width;
	int height;
	Vei2 position = Vei2(0, 0);
	int nMines;
	int revealedCounter = 0;
	int flaggedCounter = 0;
	RectI rectangle;
	DigitalDisplay minesLeftDisplay;


};