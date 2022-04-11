#include "Tile.h"
#include "SpikesTile.h"
#include "SpringTile.h"
#include "FragileTile.h"
#include "BreakableTile.h"

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
		case 65:
		case 66:
		case 81:
		case 82: {
			return new BreakableTile(id, pos, size);
		}
		case 305:
		case 306:
		case 307: {
			return new FragileTile(id, pos, size);
		}
		case 225:
		case 226:
		case 227:
		case 228: {
			return new SpikesTile(id, pos, size, SpikesTile::Orientation(id - 225));
		}
		case 145:
		case 146: {
			return new SpringTile(id, pos, size);
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