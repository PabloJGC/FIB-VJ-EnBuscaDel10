#ifndef _FRAGILE_TILE_INCLUDE
#define _FRAGILE_TILE_INCLUDE

#include "Tile.h"

class FragileTile : public Tile {
public:
	FragileTile(int id, glm::ivec2 pos, int size);

	void update(int deltaTime);
	void render() const;
	void init(ShaderProgram& shaderProgram);

	void setBreaking();
	void setBlocked();

	bool isFragile() const { return true; }
	bool isDynamic() const { return true; }
private:
	enum SpringState {
		NORMAL, BREAKING, BROKEN, BLOCKED	// BLOCKED se usa si se debe regenerar el bloque pero el jugador está en medio.
	} state;

	int timer;
	Sprite* sprite;
	Texture spritesheet;
	glm::ivec2 spriteSize;
};

#endif _FRAGILE_TILE_INCLUDE
