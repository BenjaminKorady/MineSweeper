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

void Minefield::Tile::spawnMine()
{
	assert(mine == false);
	mine = true;
}

bool Minefield::Tile::reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
	if (hasMine()) {
		return false;
	}
	return true;
}

void Minefield::Tile::flag()
{
	if (state == State::Hidden) {
		state = State::Flagged;
	}
	else if (state == State::Flagged) {
		state = State::Hidden;
	}
}

Minefield::Tile::State Minefield::Tile::getState() const
{
	return state;
}

bool Minefield::Tile::hasMine() const
{
	return mine;
}

Minefield::Minefield(int nMinesIn)
	:
	nMines(nMinesIn)
{
	assert(nMines > 0 && nMines < width*height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].setPosition(Vei2(x*Tile::width, y*Tile::width));
		}
	}

	for (int spawnedMines = 0; spawnedMines < nMinesIn; ++spawnedMines) {
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

}

void Minefield::draw(Graphics & gfx)
{
	RectI background(position, width*Tile::width, height*Tile::height);
	gfx.DrawRect(background, SpriteCodex::baseColor);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].draw(gfx, isExploded);
		}
	}
}

Vei2 Minefield::getTileLocation(const Vei2& clickLocation) const
{
	Vei2 tileLocation;
	tileLocation.x = clickLocation.x / Tile::width;
	tileLocation.y = clickLocation.y / Tile::height;
	return tileLocation;
}

void Minefield::revealTileAt(Vei2 & clickLocation)
{
	if(!isExploded) {
		Vei2 tileLocation;
		tileLocation = getTileLocation(clickLocation);
		Tile& tileAtLocation = field[tileLocation.y * width + tileLocation.x];

		if (tileAtLocation.getState() == Minefield::Tile::State::Hidden) {
			bool okayReveal = tileAtLocation.reveal();
			if (!okayReveal) {
				isExploded = true;
			}
			else {
				++revealedCounter;
			}
		}
	}
}

void Minefield::flagTileAt(Vei2 & clickLocation)
{
	if (!isExploded) {
		Vei2 tileLocation;
		tileLocation = getTileLocation(clickLocation);
		field[tileLocation.y * width + tileLocation.x].flag();
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

bool Minefield::revealedAll() const
{
	int nonMineTiles = width*height - nMines;
	assert(revealedCounter <= nonMineTiles);
	return revealedCounter == nonMineTiles;
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