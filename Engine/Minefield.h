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
#include "SpriteCodex.h"

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
		void setAdjacentMineCount(int countIn);
		void setMine(bool set);
		void setState(State stateIn);
		void restart();

		State getState() const;
		bool hasMine() const;
		int getAdjacentMineCount() const;
		Vei2 getPosition() const;

		static constexpr int size = SpriteCodex::tileSize;

	private:
		Vei2 position;
		int adjacentMineCount;
		State state = State::Hidden;
		bool mine = false;
	};

public:
	Minefield() = default;
	Minefield(int widthIn, int heightIn, int nMinesIn);
	Minefield(const Menu& menu);

	void partiallyRevealTileAtLocation(const Vei2& globalLocation);
	void revealTileAtLocation(const Vei2& globalLocation);
	void revealSurroundingTilesOrFlagTileAtLocation(const Vei2& globalLocation);
	void toggleTileFlagAtLocation(const Vei2& globalLocation);
	void hidePartiallyRevealedTile();
	void restart();

	void draw(Graphics& gfx) const;
	bool revealedAll() const;
	bool tileExistsAtLocation(const Vei2& globalLocation) const;
	bool tileAtLocationIsPartiallyRevealed(const Vei2& globalLocation) const;
	int getRevealedCounter() const;
	int getWidth() const;
	int getHeight() const;

	bool isExploded = false;
	static constexpr int displayOffset = 5;

private:
	bool minesAreGenerated = false;

private:
	Vei2 getTileLocation(const Vei2& globalLocation) const;
	Tile& getTileAtLocation(const Vei2& globalPosition);
	const Tile& getTileAtLocation(const Vei2& globalPosition) const;

	int getAdjacentMineCount(const Tile& tileIn) const;
	Vei2 getTileBox3x3Start(const Tile& tileIn) const;
	Vei2 getTileBox3x3End(const Tile& tileIn) const;
	void updateDisplay();
	void generateMines(Tile& clickedTile);
	void clearMines();
	void revealRecursively(Tile& tileIn);
	bool revealSurroundingTiles(Tile& tileIn);
	const Tile& tileAt(const Vei2& tileLocation) const;
	Tile& tileAt(const Vei2& tileLocation);

	Tile* field = nullptr;
	Tile* partiallyRevealedTilePtr = nullptr; // Keeps track of the tile that is partially revealed
	int width;
	int height;
	int nMines;
	int revealedCounter = 0;
	int flaggedCount = 0;
	RectI rectangle; // Rectangle representing the minefield (location, dimensions)
	DigitalDisplay minesLeftDisplay;


};