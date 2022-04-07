#include "SpikesTile.h"

#define SPIKES_HEIGHT 12	// La hitbox de los pinchos es ligeramente más pequeña que la textura de los pinchos para darle ventaja al jugador.

SpikesTile::SpikesTile(int id, glm::ivec2 pos, int size, Orientation orientation) : Tile(id, pos, size) {
	this->orientation = orientation;
}

bool SpikesTile::isDeathZone(glm::ivec2 playerPos, glm::ivec2 playerSize) const {
	switch (orientation) {
		case UP: {
			return playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + size &&
				playerPos.y + playerSize.y >= pos.y + size - SPIKES_HEIGHT && playerPos.y <= pos.y + size;
		}
		case RIGHT: {
			return playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + size - (size - SPIKES_HEIGHT) &&
				playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + size;
		}
		case DOWN: {
			return playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + size &&
				playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + size - (size - SPIKES_HEIGHT);
		}
		case LEFT: {
			return playerPos.x + playerSize.x >= pos.x + size - SPIKES_HEIGHT && playerPos.x <= pos.x + size &&
				playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + size;
		}
	}
}