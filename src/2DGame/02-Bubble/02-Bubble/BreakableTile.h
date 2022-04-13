#ifndef _BREAKABLE_TILE_INCLUDE
#define _BREAKABLE_TILE_INCLUDE

#include "Tile.h"

class BreakableTile : public Tile {
	public:
		BreakableTile(int id, glm::ivec2 pos, int size);

		void render() const;
		void init(ShaderProgram& shaderProgram);

		void setBroken();

		bool isBreakable() const { return state == NORMAL; }
		bool isDynamic() const { return true; }
	private:
		enum SpringState {
			NORMAL, BROKEN 
		} state;

		Sprite* sprite;
		Texture spritesheet;
		glm::ivec2 spriteSize;
};

#endif _BREAKABLE_TILE_INCLUDE
