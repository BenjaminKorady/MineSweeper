#include "Minefield.h"
#include "SpriteCodex.h"
#include "RectI.h"
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
		SpriteCodex::drawTile0(position, gfx);
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

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			field[y*width + x].setPosition(Vei2(x*Tile::width, y*Tile::width));
		}
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
