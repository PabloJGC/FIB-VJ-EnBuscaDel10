#include "Tile.h"

Tile::Tile(int id) {
	this->id = id;
	switch (id) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 8:
		case 9:
		case 10:
		case 24:
		case 25:
		case 26:
		case 145:
		case 146:
		case 161:
		case 162:
		case 177:
		case 178:
		case 193:
		case 225:
		case 226:
		case 227:
		case 228:
			transparent = true;
			break;
		default:
			transparent = false;
			break;
	}
}

bool Tile::isTransparent() const
{
	return transparent;
}

int Tile::getId() const
{
	return id;
}