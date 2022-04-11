#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"

class Tile {

	public:
		static Tile* createTile(int id, glm::ivec2 pos, int size);
		Tile(int id, glm::ivec2 pos, int size);
		bool isTransparent() const;
		int getId() const;
		virtual bool isDynamic() const { return false; }
		virtual bool isDeathZone(glm::ivec2 playerPos, glm::ivec2 playerSize) const;
		virtual bool isSpring() const { return false; }
		virtual bool isFragile() const { return false; }
		virtual bool isBreakable() const { return false; }
		virtual void init(ShaderProgram& shaderProgram) {}
		virtual void update(int deltaTime) { }
		virtual void render() const { }

	protected:
		bool transparent;
		glm::ivec2 pos;
		int id, size;
};

#endif // _TILE_INCLUDE