#include "Minefield.h"
#include "RectI.h"
#include <random>
#include <algorithm>
#include <assert.h>

/**
	Draws a Tile to the screen (If minefield is exploded, draws hidden mines as well)

	@param gfx Graphics processor
	@param isExploded Whether or not the minefield is already exploded
*/
void Minefield::Tile::draw(Graphics & gfx, bool minefieldIsExploded) const
{
	switch (state) {
	case State::Hidden:
		if (hasMine() && minefieldIsExploded) {
			SpriteCodex::drawTileMine(position, gfx);
		}
		SpriteCodex::drawTileButton(position, gfx);
		break;
	case State::PartiallyRevealed:
		SpriteCodex::drawTile0(position, gfx);
		break;
	case State::Revealed:
		if (!hasMine()) {
			SpriteCodex::drawTileNumber(adjacentMineCount, position, gfx);
		}
		else /* hasMine */ {
			minefieldIsExploded ?
				SpriteCodex::drawTileMineRed(position, gfx) : SpriteCodex::drawTileButton(position, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::drawTileButton(position, gfx);
		SpriteCodex::drawTileFlag(position, gfx);
		if (minefieldIsExploded && !hasMine()) {
			SpriteCodex::drawTileCross(position, gfx);
		}
		break;
	}
}

/**
	Returns the position of the Tile

	@return position
*/
Vei2 Minefield::Tile::getPosition() const
{
	return position;
}

/**
	Sets the position of the Tile

	@param position
*/
void Minefield::Tile::setPosition(Vei2 position)
{
	this->position = position;
}

/**
	Sets the number of mines which are adjacent to the tile

	@param mineCount 
*/
void Minefield::Tile::setAdjacentMineCount(int mineCount)
{
	adjacentMineCount = mineCount;
}

/**
	Sets whether or not the tile contains a mine

	@param set mine or !mine
*/
void Minefield::Tile::setMine(bool set)
{
	if (set) {
		assert(!mine);
		mine = true;
	}
	else {
		assert(mine);  
		mine = false;
	}
}

/**
	Sets the state of the tile

	@param stateIn
*/
void Minefield::Tile::setState(State stateIn)
{
	switch (stateIn) {
	case State::Revealed:
		assert(state == State::Hidden || state == State::PartiallyRevealed);
		break;
	case State::PartiallyRevealed:
		assert(state == State::Hidden);
		break;
	case State::Hidden:
		assert(state != State::Hidden);
		break;
	case State::Flagged:
		assert(state == State::Hidden);
		break;
	}
	state = stateIn;
}

/**
	Returns the amount of mines adjacent to the tile

	@return adjacentMineCount
*/
int Minefield::Tile::getadjacentMineCount() const
{
	return adjacentMineCount;
}

/**
	Returns the state of the tile

	@return state
*/
Minefield::Tile::State Minefield::Tile::getState() const
{
	return state;
}

/**
	Returns true if the tile has a mine

	@return mine
*/
bool Minefield::Tile::hasMine() const
{
	return mine;
}

/**
	Restarts the Tile to default states
*/
void Minefield::Tile::restart()
{
	if (state != State::Hidden)
		setState(State::Hidden);
	if (hasMine()) {
		setMine(false);
	}
}

/**
	Constructs a minefield object

	@param widthIn Width of the field (in tiles)
	@param heightIn Height of the field (in tiles)
	@param nMinesIn The amount of mines for the field to contain
*/
Minefield::Minefield(int widthIn, int heightIn, int nMinesIn)
	:
	width(widthIn),
	height(heightIn),
	nMines(nMinesIn),
	minesLeftDisplay(DigitalDisplay(nMines))
{
	assert(nMines > 0 && nMines < width*height);
	assert(Graphics::ScreenWidth >= Tile::size * widthIn);
	assert(Graphics::ScreenHeight >= Tile::size * heightIn);
	restart();
}

/**
	Constructs a minefield object based on the currently selected menu choice

	@param menu A menu object with chosen current difficulty
*/
Minefield::Minefield(const Menu& menu)
{
	Menu::Option::Name difficulty = menu.getSelectedOption();

	assert(difficulty != Menu::Option::Name::None);	//Ensure a difficulty is selected

	int fieldWidth = menu.options[(int)difficulty].setsMinefieldSize.x;
	int fieldHeight = menu.options[(int)difficulty].setsMinefieldSize.y;
	int mineCount = menu.options[(int)difficulty].setsMines;
	 
	*this = Minefield(fieldWidth, fieldHeight, mineCount);
}

/**
	Generates mines randomly accross the field after a tile was clicked
*/
void Minefield::generateMines(Tile& clickedTile)
{
	assert(!minesAreGenerated);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	do {
		clearMines();

		for (int spawnedMines = 0; spawnedMines < nMines; ++spawnedMines) {
			Vei2 spawnPosition;
			// Keep generating a new spawn position while current position already has a mine
			do {
				spawnPosition = { xDist(rng), yDist(rng) };
			} while (tileAt(spawnPosition).hasMine());

			tileAt(spawnPosition).setMine(true);
		}

		// Once mines have been spawned, set the numbers of each tile stating how many mines are nearby
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int n = getadjacentMineCount(field[y*width + x]);
				field[y*width + x].setAdjacentMineCount(n);
			}
		}

	// Keep generating a new minefield unless clicked tile generates with 0 adjacent mines or has a mine (allow dying on first click because that's always fun) 
	} while ((getadjacentMineCount(clickedTile) != 0)  && !clickedTile.hasMine());
	
	minesAreGenerated = true;
}


/**
	Clears the field of all mines
*/
void Minefield::clearMines()
{
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (field[y*width + x].hasMine()) {
				field[y*width + x].setMine(false);
			}
		}
	}
}

/**
	Draws the minefield

	@param gfx Graphics processor
*/
void Minefield::draw(Graphics & gfx)
{
	// Background rectangle
	gfx.DrawRect(rectangle, SpriteCodex::baseColor);

	// Tiles
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].draw(gfx, isExploded);
		}
	}

	// Display
	minesLeftDisplay.draw(gfx, field->getPosition().x, field->getPosition().y - DigitalDisplay::getHeight() - displayOffset);
}

/**
	Reveals a tile partially
*/
void Minefield::partiallyRevealTile(Tile & tileIn)
{
	tileIn.setState(Tile::State::PartiallyRevealed);
}

void Minefield::partiallyRevealTileAt(Vei2 & globalLocation)
{
	const Vei2 tileLocation = getTileLocation(globalLocation);
	Tile& tile = tileAt(tileLocation);
	if (tile.getState() == Tile::State::Hidden) {
		tile.setState(Tile::State::PartiallyRevealed);
		partiallyRevealedTilePtr = &tile;
	}
}

Vei2 Minefield::getTileLocation(const Vei2& globalLocation) const
{
	assert(rectangle.ContainsPoint(globalLocation));
	Vei2 tileLocation;
	tileLocation.x = (globalLocation.x - field[0].getPosition().x) / Tile::size;
	tileLocation.y = (globalLocation.y - field[0].getPosition().y) / Tile::size;
	return tileLocation;
}

bool Minefield::tileExistsAtLocation(const Vei2& globalLocation) const
{
	return rectangle.ContainsPoint(globalLocation);
}

bool Minefield::tileAtLocationIsPartiallyRevealed(const Vei2 & globalLocation) const
{
	const Vei2 tileLocation = getTileLocation(globalLocation);
	const Tile& tile = tileAt(tileLocation);

	return tile.getState() == Tile::State::PartiallyRevealed;
}

int Minefield::getFlaggedCounter() const
{
	return flaggedCounter;
}

int Minefield::getMineCounter() const
{
	return nMines;
}

void Minefield::revealTileAt(Vei2 & globalLocation)
{
	if(!isExploded) {
		Vei2 tileLocation;
		tileLocation = getTileLocation(globalLocation);
		Tile& tileAtLocation = field[tileLocation.y * width + tileLocation.x];

		if (tileAtLocation.getState() == Minefield::Tile::State::Hidden ||
			tileAtLocation.getState() == Minefield::Tile::State::PartiallyRevealed) {
			bool okayReveal = !tileAtLocation.hasMine();
			if (!okayReveal) {
				isExploded = true;
			}
			revealRecursively(tileAtLocation);
			partiallyRevealedTilePtr = nullptr;
		}
	}
}

void Minefield::revealRecursively(Tile & tileIn)
{
	if (getRevealedCounter() == 0) {
		generateMines(tileIn);
	}
	if (tileIn.getState() == Tile::State::Hidden ||
		tileIn.getState() == Tile::State::PartiallyRevealed) {
		tileIn.setState(Tile::State::Revealed);
		if (tileIn.hasMine()) {
			isExploded = true;
			return;
		}
		else {
			++revealedCounter;
		}
		if (tileIn.getadjacentMineCount() != 0) {
			return;
		}
		else {

			Vei2 revealStart;
			Vei2 revealEnd;

			Vei2 tileLocal = getTileLocation(tileIn.getPosition());

			revealStart.x = std::max(0, tileLocal.x - 1);
			revealEnd.x = std::min(tileLocal.x + 1, width - 1);
			revealStart.y = std::max(0, tileLocal.y - 1);
			revealEnd.y = std::min(tileLocal.y + 1, height - 1);

			int mineCounter = 0;

			for (int y = revealStart.y; y <= revealEnd.y; ++y) {
				for (int x = revealStart.x; x <= revealEnd.x; ++x) {
					Tile& adjacentTile = field[y*width + x];
					revealRecursively(adjacentTile);
				}
			}
		}
	}
}

bool Minefield::revealSurrounding(Tile & tileIn)
{
	Vei2 revealStart;
	Vei2 revealEnd;

	Vei2 tileLocal = getTileLocation(tileIn.getPosition());

	revealStart.x = std::max(0, tileLocal.x - 1);
	revealEnd.x = std::min(tileLocal.x + 1, width - 1);
	revealStart.y = std::max(0, tileLocal.y - 1);
	revealEnd.y = std::min(tileLocal.y + 1, height - 1);

	int mineCounter = 0;

	// Count flagged
	int surroundingFlaggedCounter = 0;
	for (int y = revealStart.y; y <= revealEnd.y; ++y) {
		for (int x = revealStart.x; x <= revealEnd.x; ++x) {
			Tile& adjacentTile = field[y*width + x];
			if (adjacentTile.getState() == Tile::State::Flagged) {
				++surroundingFlaggedCounter;
			}
		}
	}

	// It's okay to reveal surrounding mines
	if (surroundingFlaggedCounter == tileIn.getadjacentMineCount()) {
		for (int y = revealStart.y; y <= revealEnd.y; ++y) {
			for (int x = revealStart.x; x <= revealEnd.x; ++x) {
				Tile& adjacentTile = field[y*width + x];
				if(adjacentTile.getState() == Tile::State::Hidden) {
					revealRecursively(adjacentTile);
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void Minefield::revealOrFlagAt(const Vei2 & globalLocation)
{
	Vei2 tileLocation;
	tileLocation = getTileLocation(globalLocation);
	Tile& tileIn = tileAt(tileLocation);
	if (tileIn.getState() == Tile::State::Revealed) {
		revealSurrounding(tileIn);
	}
	else if (tileIn.getState() == Tile::State::Hidden || tileIn.getState() == Tile::State::Flagged) {
		flagTileAt(globalLocation);		
	}
}

void Minefield::flagTileAt(const Vei2 & globalLocation)
{
	if (!isExploded) {
		Vei2 tileLocation;
		tileLocation = getTileLocation(globalLocation);
		int i = tileLocation.y * width + tileLocation.x;
			if (field[i].getState() == Tile::State::Hidden) {
				field[i].setState(Tile::State::Flagged);
				++flaggedCounter;

			}
			else if (field[i].getState() == Tile::State::Flagged) {
				field[i].setState(Tile::State::Hidden);
				--flaggedCounter;
			}
			else {
				assert(false);	// State was invalid to begin with
			}
		updateDisplay();
	}
}

const Minefield::Tile & Minefield::tileAt(const Vei2 & tileLocation) const
{
	return field[tileLocation.y * width + tileLocation.x];
}

Minefield::Tile & Minefield::tileAt(const Vei2 & tileLocation)
{
	return field[tileLocation.y * width + tileLocation.x];
}

void Minefield::hidePartiallyRevealedTile()
{
	if (partiallyRevealedTilePtr != nullptr) {
		partiallyRevealedTilePtr->setState(Tile::State::Hidden);
		partiallyRevealedTilePtr = nullptr;
	}
}

bool Minefield::revealedAll() const
{
	int nonMineTiles = width*height - nMines;
	assert(revealedCounter <= nonMineTiles);
	return revealedCounter == nonMineTiles;
}

int Minefield::getRevealedCounter() const
{
	return revealedCounter;
}

int Minefield::getWidth() const
{
	return width * Tile::size;
}

int Minefield::getHeight() const
{
	return height * Tile::size;
}

void Minefield::restart()
{
	minesAreGenerated = false;

	if (field!=nullptr) {
		delete[] field;
		field = nullptr;
	}
	field = new Tile[width*height];

	isExploded = false;

	Vei2 centerTopLeft = { (Graphics::ScreenWidth - width * Tile::size) / 2, (Graphics::ScreenHeight - height * Tile::size) / 2 };

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].restart();
			field[y*width + x].setPosition(Vei2(x*Tile::size + centerTopLeft.x, y*Tile::size + centerTopLeft.y));
		}
	}

	rectangle = RectI(field[0].getPosition(), width*Tile::size, height*Tile::size);

	revealedCounter = 0;
}

int Minefield::getadjacentMineCount(const Tile& tileIn) const
{
	
	Vei2 checkStart;
	Vei2 checkEnd;

	Vei2 tileLocal = getTileLocation(tileIn.getPosition());

	checkStart.x = std::max(0, tileLocal.x - 1);
	checkStart.y = std::max(0, tileLocal.y - 1);
	checkEnd.x = std::min(tileLocal.x + 1, width - 1);
	checkEnd.y = std::min(tileLocal.y + 1, height - 1);

	int mineCounter = 0;

	for (int y = checkStart.y; y <= checkEnd.y; ++y) {
		for (int x = checkStart.x; x <= checkEnd.x; ++x) {
			if (field[y*width + x].hasMine()) {
				++mineCounter;
			}
		}
	}

	return mineCounter;
}

void Minefield::updateDisplay()
{
	minesLeftDisplay = DigitalDisplay(nMines - flaggedCounter);
}
