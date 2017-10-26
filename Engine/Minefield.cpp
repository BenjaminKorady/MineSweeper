#include "Minefield.h"
#include "SpriteCodex.h"
#include "RectI.h"
#include <random>
#include <algorithm>
#include <assert.h>


Minefield::Tile::Tile()
	:
	position(Vei2(0,0))
{
}

Minefield::Tile::Tile(Vei2 posIn)
	:
	position(posIn)
{
}

void Minefield::Tile::draw(Graphics & gfx, bool isExploded) const
{
	if (!isExploded) {

		switch (state) {
		case State::Hidden:
			SpriteCodex::drawTileButton(position, gfx);
			break;
		case State::PartiallyRevealed: {
			SpriteCodex::drawTile0(position, gfx); 
		} break;
		case State::Revealed:
			if (hasMine()) {
				SpriteCodex::drawTileMine(position, gfx);
			}
			else {
				switch (adjacentMines) {
				case 0:
					SpriteCodex::drawTile0(position, gfx);
					break;
				case 1:
					SpriteCodex::drawTile1(position, gfx);
					break;
				case 2:
					SpriteCodex::drawTile2(position, gfx);
					break;
				case 3:
					SpriteCodex::drawTile3(position, gfx);
					break;
				case 4:
					SpriteCodex::drawTile4(position, gfx);
					break;
				case 5:
					SpriteCodex::drawTile5(position, gfx);
					break;
				case 6:
					SpriteCodex::drawTile6(position, gfx);
					break;
				case 7:
					SpriteCodex::drawTile7(position, gfx);
					break;
				case 8:
					SpriteCodex::drawTile8(position, gfx);
					break;
				}
			}
			break;
		case State::Flagged:
			SpriteCodex::drawTileButton(position, gfx);
			SpriteCodex::drawTileFlag(position, gfx);
			break;
		}
	}

	else {
		switch (state) {
		case State::Hidden: {
			if (hasMine()) {
				SpriteCodex::drawTileMine(position, gfx);
			}
			SpriteCodex::drawTileButton(position, gfx);
			break;
		}
		case State::Revealed: {
			if (hasMine()) {
				SpriteCodex::drawTileMineRed(position, gfx);
			}
			else {
				switch (adjacentMines) {
				case 0:
					SpriteCodex::drawTile0(position, gfx);
					break;
				case 1:
					SpriteCodex::drawTile1(position, gfx);
					break;
				case 2:
					SpriteCodex::drawTile2(position, gfx);
					break;
				case 3:
					SpriteCodex::drawTile3(position, gfx);
					break;
				case 4:
					SpriteCodex::drawTile4(position, gfx);
					break;
				case 5:
					SpriteCodex::drawTile5(position, gfx);
					break;
				case 6:
					SpriteCodex::drawTile6(position, gfx);
					break;
				case 7:
					SpriteCodex::drawTile7(position, gfx);
					break;
				case 8:
					SpriteCodex::drawTile8(position, gfx);
					break;
				}
			}
			}
			break;
		case State::Flagged: {
			if (hasMine()) {
				SpriteCodex::drawTileButton(position, gfx);
				SpriteCodex::drawTileFlag(position, gfx);
			}
			else {
				SpriteCodex::drawTileButton(position, gfx);
				SpriteCodex::drawTileFlag(position, gfx);
				SpriteCodex::drawTileCross(position, gfx);
			}
			break;
		}
		}
	}
}

Vei2 Minefield::Tile::getPosition() const
{
	return position;
}

void Minefield::Tile::setPosition(Vei2 posIn)
{
	position = posIn;
}

void Minefield::Tile::setAdjacentMines(int in)
{
	adjacentMines = in;
}

int Minefield::Tile::getAdjacentMines() const
{
	return adjacentMines;
}

void Minefield::Tile::spawnMine()
{
	assert(mine == false);
	mine = true;
}

void Minefield::Tile::clearMine()
{
	mine = false;
}

void Minefield::Tile::reveal()
{
	assert(state == State::Hidden || state == State::PartiallyRevealed);
	state = State::Revealed;
}

void Minefield::Tile::partiallyReveal()
{
	state = State::PartiallyRevealed;
}

void Minefield::Tile::hide()
{
	state = State::Hidden;
}

bool Minefield::Tile::flag()
{
	if (state == State::Hidden) {
		state = State::Flagged;
		return true;
	}
	else if (state == State::Flagged) {
		state = State::Hidden;
		return false;
	}

	return false;
}

Minefield::Tile::State Minefield::Tile::getState() const
{
	return state;
}

bool Minefield::Tile::hasMine() const
{
	return mine;
}

void Minefield::Tile::restart()
{
	state = State::Hidden;
	mine = false;
}

Minefield::Minefield(int widthIn, int heightIn, int nMinesIn)
	:
	width(widthIn),
	height(heightIn),
	nMines(nMinesIn),
	minesLeftDisplay(DigitalDisplay(nMines))
{
	assert(nMines > 0 && nMines < width*height);
	assert(Graphics::ScreenWidth >= Tile::width * widthIn);
	assert(Graphics::ScreenHeight >= Tile::height * heightIn);
	restart();
}

Minefield::Minefield(const Menu& menu)
{
	Menu::Option::Name difficulty = menu.getSelectedOption();

	assert(difficulty != Menu::Option::Name::None);

	int fieldWidth = menu.options[(int)difficulty].setsMinefieldSize.x;
	int fieldHeight = menu.options[(int)difficulty].setsMinefieldSize.y;
	int mineCount = menu.options[(int)difficulty].setsMines;
	 
	*this = Minefield(fieldWidth, fieldHeight, mineCount);
}

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
			do {
				spawnPosition = { xDist(rng), yDist(rng) };
			} while (tileAt(spawnPosition).hasMine());

			tileAt(spawnPosition).spawnMine();
		}

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int n = getAdjacentMines(field[y*width + x]);
				field[y*width + x].setAdjacentMines(n);
			}
		}
	// Keep generating a new minefield unless clicked tile generates with 0 adjacent mine or has mine (allow dying on first click because that's always fun) 
	} while ((getAdjacentMines(clickedTile) != 0)  && !clickedTile.hasMine());
	
	minesAreGenerated = true;
}

void Minefield::clearMines()
{
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].clearMine();
		}
	}
}

void Minefield::draw(Graphics & gfx)
{
	gfx.DrawRect(rectangle, SpriteCodex::baseColor);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].draw(gfx, isExploded);
		}
	}
	minesLeftDisplay.draw(gfx, field->getPosition().x, field->getPosition().y - DigitalDisplay::digitHeight - displayOffset);
}

void Minefield::partiallyRevealTile(Tile & tileIn)
{
	tileIn.partiallyReveal();
}

void Minefield::partiallyRevealTileAt(Vei2 & globalLocation)
{
	const Vei2 tileLocation = getTileLocation(globalLocation);
	Tile& tile = tileAt(tileLocation);
	if (tile.getState() == Tile::State::Hidden) {
		tile.partiallyReveal();
		partiallyRevealedTilePtr = &tile;
	}
}

Vei2 Minefield::getTileLocation(const Vei2& globalLocation) const
{
	assert(rectangle.ContainsPoint(globalLocation));
	Vei2 tileLocation;
	tileLocation.x = (globalLocation.x - field[0].getPosition().x) / Tile::width;
	tileLocation.y = (globalLocation.y - field[0].getPosition().y) / Tile::height;
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
		tileIn.reveal();
		if (tileIn.hasMine()) {
			isExploded = true;
			return;
		}
		else {
			++revealedCounter;
		}
		if (tileIn.getAdjacentMines() != 0) {
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
	if (surroundingFlaggedCounter == tileIn.getAdjacentMines()) {
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
		if (field[tileLocation.y * width + tileLocation.x].flag()) {
			++flaggedCounter;
		}
		else {
			--flaggedCounter;
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
		partiallyRevealedTilePtr->hide();
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
	return width * Tile::width;
}

int Minefield::getHeight() const
{
	return height * Tile::height;
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

	Vei2 centerTopLeft = { (Graphics::ScreenWidth - width * Tile::width) / 2, (Graphics::ScreenHeight - height * Tile::height) / 2 };

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].restart();
			field[y*width + x].setPosition(Vei2(x*Tile::width + centerTopLeft.x, y*Tile::width + centerTopLeft.y));
		}
	}

	rectangle = RectI(field[0].getPosition(), width*Tile::width, height*Tile::height);

	revealedCounter = 0;
}

int Minefield::getAdjacentMines(const Tile& tileIn) const
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
