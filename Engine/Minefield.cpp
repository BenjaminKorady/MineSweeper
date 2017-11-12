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
int Minefield::Tile::getAdjacentMineCount() const
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
				int n = getAdjacentMineCount(field[y*width + x]);
				field[y*width + x].setAdjacentMineCount(n);
			}
		}

	// Keep generating a new minefield unless clicked tile generates with 0 adjacent mines 
	} while ((getAdjacentMineCount(clickedTile) != 0) );
	
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
void Minefield::draw(Graphics & gfx) const
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
	Partially reveals a tile at given location

	@param globalLocation Location where the tile to be revealed is
*/
void Minefield::partiallyRevealTileAtLocation(const Vei2 & globalLocation)
{
	Tile& tile = getTileAtLocation(globalLocation);
	if (tile.getState() == Tile::State::Hidden) {
		tile.setState(Tile::State::PartiallyRevealed);
		partiallyRevealedTilePtr = &tile;	// Store the last partially revealed tile address in a pointer
	}
}

/**
	Converts a global location input to a tile location of the grid (e.g. tile at {324, 450} could be tile[0][3], this will return {0, 3})

	@return tileLocation
*/
Vei2 Minefield::getTileLocation(const Vei2& globalLocation) const
{
	assert(rectangle.ContainsPoint(globalLocation));
	Vei2 tileLocation;
	tileLocation.x = (globalLocation.x - field[0].getPosition().x) / Tile::size;
	tileLocation.y = (globalLocation.y - field[0].getPosition().y) / Tile::size;
	return tileLocation;
}

/**
	Returns true if tile exists at input location (Normally used to check if mouse click was within the minefield boundaries)

	@param globalLocation
	@return bool
*/
bool Minefield::tileExistsAtLocation(const Vei2& globalLocation) const
{
	return rectangle.ContainsPoint(globalLocation);
}

/**
	Returns true if tile at input location is partially revealed

	@param globalLocation
	@return bool
*/
bool Minefield::tileAtLocationIsPartiallyRevealed(const Vei2 & globalLocation) const
{
	const Tile& tile = getTileAtLocation(globalLocation);
	return tile.getState() == Tile::State::PartiallyRevealed;
}

/**
	Reveals a tile at input location
	
	@param globalLocation
*/
void Minefield::revealTileAtLocation(const Vei2 & globalLocation)
{
	if(!isExploded) {
		Tile& tile = getTileAtLocation(globalLocation);

		if (tile.getState() == Tile::State::Hidden || tile.getState() == Tile::State::PartiallyRevealed) {
			if (tile.hasMine()) {
				isExploded = true;
			}

			revealRecursively(tile);	// Recursive reveal for tiles with 0 adjacent miness
			if (partiallyRevealedTilePtr != nullptr) {
				partiallyRevealedTilePtr = nullptr;		// Reset pointer (No partially revealed tile exists if we are revealing)
			}
		}
	}
}

/**
	Reveals input tile and if it has 0 adjacent mines, also reveal all surrounding tiles recursively
*/
void Minefield::revealRecursively(Tile & tileIn)
{
	if (getRevealedCounter() == 0) {
		generateMines(tileIn);	// Mines are generated after first click
	}
	if (tileIn.getState() == Tile::State::Hidden || tileIn.getState() == Tile::State::PartiallyRevealed) {
		tileIn.setState(Tile::State::Revealed);
		if (tileIn.hasMine()) {
			isExploded = true;
			return;
		}
		else {
			++revealedCounter;
		}

		if (tileIn.getAdjacentMineCount() == 0) {
			// If adjacent mines is 0, we need to reveal all surrounding tiles

			// Reveal tiles in a 3x3 box, unless the clicked tile is in a corner / near the wall edge, 
			// then the reveal box will be smaller, capped by the edges
			Vei2 revealStart = getTileBox3x3Start(tileIn);
			Vei2 revealEnd = getTileBox3x3End(tileIn);

			for (int y = revealStart.y; y <= revealEnd.y; ++y) {
				for (int x = revealStart.x; x <= revealEnd.x; ++x) {
					Tile& adjacentTile = field[y*width + x];
					revealRecursively(adjacentTile);
				}
			}
		}
	}
}

/**
	Reveals all surrounding tiles (called  on Left + Right mouse click / Spacebar press on a revealed tile)

	@param tileIn tile of which the surrounding tiles should be revealed
	@return bool true if successfully revealed surrounding tiles, false otherwise (E.g. false if the user left + right clicked a number 4 tiles which only had 3 flags nearby)
*/
bool Minefield::revealSurroundingTiles(Tile & tileIn)
{
	// Reveal tiles in a 3x3 box, unless the clicked tile is in a corner / near the wall edge, 
	// then the reveal box will be smaller, capped by the edges
	Vei2 revealStart = getTileBox3x3Start(tileIn);
	Vei2 revealEnd = getTileBox3x3End(tileIn);

	// Count surrounding flags
	int surroundingFlagsCount = 0;
	for (int y = revealStart.y; y <= revealEnd.y; ++y) {
		for (int x = revealStart.x; x <= revealEnd.x; ++x) {
			const Tile& adjacentTile = field[y*width + x];
			if (adjacentTile.getState() == Tile::State::Flagged) {
				++surroundingFlagsCount;
			}
		}
	}

	// It's okay to reveal surrounding mines
	if (surroundingFlagsCount == tileIn.getAdjacentMineCount()) {
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

/**
	Reveals tiles surrounding the tile at input location if it is revealed, flags the tile otherwise

	@param globalLocation Location of the tile to be flagged / have surrounding tiles revealed
*/
void Minefield::revealSurroundingTilesOrFlagTileAtLocation(const Vei2 & globalLocation)
{
	Tile& tileIn = getTileAtLocation(globalLocation);
	if (tileIn.getState() == Tile::State::Revealed) {
		revealSurroundingTiles(tileIn);
	}
	else if (tileIn.getState() == Tile::State::Hidden || tileIn.getState() == Tile::State::Flagged) {
		toggleTileFlagAtLocation(globalLocation);		
	}
}

/**
	Toggles flag of tile at input location

	@param globalLocation
*/
void Minefield::toggleTileFlagAtLocation(const Vei2 & globalLocation)
{
	Tile& tile = getTileAtLocation(globalLocation);
		if (tile.getState() == Tile::State::Hidden) {
			tile.setState(Tile::State::Flagged);
			++flaggedCount;
		}
		else if (tile.getState() == Tile::State::Flagged) {
			tile.setState(Tile::State::Hidden);
			--flaggedCount;
		}
	updateDisplay();	// Display shows amount of un-flagged mines left, therefore update every time you change flag count
}

/**
	Returns a constant reference to a tile at input tile location

	@param tileLocation
	@return tile
*/
const Minefield::Tile & Minefield::tileAt(const Vei2 & tileLocation) const
{
	return field[tileLocation.y * width + tileLocation.x];
}
/**
	Returns a non-constant reference to a tile at input tile location

	@param tileLocation
	@return tile
*/
Minefield::Tile & Minefield::tileAt(const Vei2 & tileLocation)
{
	return field[tileLocation.y * width + tileLocation.x];
}

/**
	Hides the tile that was partially revealed and reset the pointer to the partially revealed tile to nullptr
*/
void Minefield::hidePartiallyRevealedTile()
{
	if (partiallyRevealedTilePtr != nullptr) {
		partiallyRevealedTilePtr->setState(Tile::State::Hidden);
		partiallyRevealedTilePtr = nullptr;
	}
}

void Minefield::flagRemainingTiles()
{
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Tile& tile = field[y*width + x];
			if (tile.hasMine() && tile.getState() == Tile::State::Hidden) {
				tile.setState(Tile::State::Flagged);
			}
		}
	}
}

/**
	Returns true if all tiles have been revealed

	@return bool
*/
bool Minefield::revealedAll() const
{
	int nonMineTiles = width*height - nMines;
	assert(revealedCounter <= nonMineTiles);
	return revealedCounter == nonMineTiles;
}

/**
	Returns the amount of revealed tiles
	
	@return revealedCounter
*/
int Minefield::getRevealedCounter() const
{
	return revealedCounter;
}

/**
	Returns the width of the minefield (in pixels)

	@return width width in tiles * tile size
*/
int Minefield::getWidth() const
{
	return width * Tile::size;
}

/**
	Returns the height of the minefield (in pixels)

	@return height height in tiles * tile size
*/
int Minefield::getHeight() const
{
	return height * Tile::size;
}

/**
	Restarts the minefield back to its default values
*/
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

/**
	Returns a non-const reference to the tile at input location
	
	@param globalLocation
	@return tile
*/
Minefield::Tile& Minefield::getTileAtLocation(const Vei2 & globalLocation)
{
	assert(rectangle.ContainsPoint(globalLocation));
	Vei2 tileLocation = getTileLocation(globalLocation);
	return tileAt(tileLocation);
}

/**
	Returns a const reference to the tile at input location
	
	@param globalLocation
	@return tile
*/
const Minefield::Tile & Minefield::getTileAtLocation(const Vei2 & globalLocation) const
{
	assert(rectangle.ContainsPoint(globalLocation));
	Vei2 tileLocation = getTileLocation(globalLocation);
	return tileAt(tileLocation);
}

/**
	Returns the amount of mines adjacent to the input tile

	@param tile
	@return adjacentMines
*/
int Minefield::getAdjacentMineCount(const Tile& tileIn) const
{
	Vei2 checkStart = getTileBox3x3Start(tileIn);
	Vei2 checkEnd = getTileBox3x3End(tileIn);

	Vei2 tileLocal = getTileLocation(tileIn.getPosition());

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

/**
	Returns the starting position (top-left point) of (usually) a 3x3 box (This box may be 2x3, 3x2, or 2x2 if clipped near a wall),
	which is surrounding the input tile

	@param tile tile which should be in the center of the 3x3 box
	@return tileLocalPosition The position of the starting tile of the clipping box
*/
Vei2 Minefield::getTileBox3x3Start(const Tile & tileIn) const
{
	Vei2 tileLocalPosition = getTileLocation(tileIn.getPosition());
	return { std::max(0, tileLocalPosition.x - 1), std::max(0, tileLocalPosition.y - 1) };
}

/**
	Returns the ending position (top-left point) of (usually) a 3x3 box (This box may be 2x3, 3x2, or 2x2 if clipped near a wall),
	which is surrounding the input tile

	@param tile tile which should be in the center of the 3x3 box
	@return tileLocalPosition The position of the ending tile of the clipping box
*/
Vei2 Minefield::getTileBox3x3End(const Tile & tileIn) const
{
	Vei2 tileLocalPosition = getTileLocation(tileIn.getPosition());
	return { std::min(tileLocalPosition.x + 1, width - 1), std::min(tileLocalPosition.y + 1, height - 1) };
}

/**
	Updates the display non-flagged mines counter display
*/
void Minefield::updateDisplay()
{
	minesLeftDisplay = DigitalDisplay(nMines - flaggedCount);
}
