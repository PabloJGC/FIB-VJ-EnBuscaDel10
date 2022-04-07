#include "Tile.h"
#include "SpikesTile.h"

Tile* Tile::createTile(int id, glm::ivec2 pos, int size) {
	switch (id) {
		/*case 65:
		case 66:
		case 81:
		case 82:
		case 145:
		case 146:
		case 193:
		case 305:*/

		case 225:
		case 226:
		case 227:
		case 228: {
			Tile* ret = new SpikesTile(id, pos, size, SpikesTile::Orientation(id - 225));
			return ret;
		}
		default: {
			return new Tile(id, pos, size);
		}
	}
}

Tile::Tile(int id, glm::ivec2 pos, int size) {
	this->id = id;
	this->pos = pos;
	this->size = size;
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

	dynamic = false;
	deathZone = false;
}

bool Tile::isTransparent() const
{
	return transparent;
}

int Tile::getId() const
{
	return id;
}

bool Tile::isDeathZone(glm::ivec2 playerPos, glm::ivec2 playerSize) const {
	return false;
}