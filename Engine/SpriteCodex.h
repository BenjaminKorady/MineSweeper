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
	static void drawTileMine( const Vei2& pos,Graphics& gfx );
	static void drawTileMineRed( const Vei2& pos,Graphics& gfx );

	static void drawGameWin(Graphics& gfx);
	static void drawGameLoss(Graphics& gfx);
	static void drawBeginner(int x, int y, Graphics& gfx);
	static void drawIntermediate(int x, int y, Graphics& gfx);
	static void drawExpert(int x, int y, Graphics& gfx);
	static void drawBeginnerGlow(int x, int y, Graphics& gfx);
	static void drawIntermediateGlow(int x, int y, Graphics& gfx);
	static void drawExpertGlow(int x, int y, Graphics& gfx);

};