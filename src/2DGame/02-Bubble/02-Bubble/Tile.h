#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>

class Tile {

	public:
		static Tile* createTile(int id, glm::ivec2 pos, int size);
		Tile(int id, glm::ivec2 pos, int size);
		bool isTransparent() const;
		int getId() const;
		virtual bool isDeathZone(glm::ivec2 playerPos, glm::ivec2 playerSize) const;

	private:
		int id;
		bool transparent;
		bool dynamic;
	protected:
		glm::ivec2 pos;
		int size;
		bool deathZone;
};

#endif // _TILE_INCLUDE