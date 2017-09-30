#include "Minefield.h"
#include "SpriteCodex.h"
#include "RectI.h"
#include <random>
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

void Minefield::Tile::draw(Graphics & gfx) const
{

	switch (state) {
	case State::Hidden:
		SpriteCodex::drawTileButton(position, gfx);
		break;
	case State::Revealed:
		if (mine) {
			SpriteCodex::drawTileBomb(position, gfx);
		}
		else {
			SpriteCodex::drawTile0(position, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::drawTileButton(position, gfx);
		SpriteCodex::drawTileFlag(position, gfx);
		break;
	}
}

void Minefield::Tile::setPosition(Vei2 posIn)
{
	position = posIn;
}

void Minefield::Tile::spawnMine()
{
	assert(mine == false);
	mine = true;
}

void Minefield::Tile::reveal()
{
	if (state == State::Hidden) {
		state = State::Revealed;
	}
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

}

void Minefield::draw(Graphics & gfx)
{
	RectI background(position, width*Tile::width, height*Tile::height);
	gfx.DrawRect(background, SpriteCodex::baseColor);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].draw(gfx);
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
	Vei2 tileLocation;
	tileLocation = getTileLocation(clickLocation);
	field[tileLocation.y * width + tileLocation.x].reveal();
}

void Minefield::flagTileAt(Vei2 & clickLocation)
{
	Vei2 tileLocation;
	tileLocation = getTileLocation(clickLocation);
	field[tileLocation.y * width + tileLocation.x].flag();
}

const Minefield::Tile & Minefield::tileAt(Vei2 & tileLocation) const
{
	return field[tileLocation.y * width + tileLocation.x];
}

Minefield::Tile & Minefield::tileAt(Vei2 & tileLocation)
{
	return field[tileLocation.y * width + tileLocation.x];
}
