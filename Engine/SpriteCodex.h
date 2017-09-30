#pragma once

#include "Graphics.h"
#include "Vei2.h"

class SpriteCodex
{
public:
	// width and height of all tiles
	static constexpr int tileSize = 16;
	// base color for all tiles
	static constexpr Color baseColor = { 192,192,192 };
	// 16x16 tile sprites assume (192,192,192) background and top left origin
	static void drawTile0( const Vei2& pos,Graphics& gfx );
	static void drawTile1( const Vei2& pos,Graphics& gfx );
	static void drawTile2( const Vei2& pos,Graphics& gfx );
	static void drawTile3( const Vei2& pos,Graphics& gfx );
	static void drawTile4( const Vei2& pos,Graphics& gfx );
	static void drawTile5( const Vei2& pos,Graphics& gfx );
	static void drawTile6( const Vei2& pos,Graphics& gfx );
	static void drawTile7( const Vei2& pos,Graphics& gfx );
	static void drawTile8( const Vei2& pos,Graphics& gfx );
	static void drawTileButton( const Vei2& pos,Graphics& gfx );
	static void drawTileCross( const Vei2& pos,Graphics& gfx );
	static void drawTileFlag( const Vei2& pos,Graphics& gfx );
	static void drawTileBomb( const Vei2& pos,Graphics& gfx );
	static void drawTileBombRed( const Vei2& pos,Graphics& gfx );
};