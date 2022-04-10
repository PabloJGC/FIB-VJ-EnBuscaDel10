#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Tile.h"
#include "SpringTile.h"
#include "FragileTile.h"
#include "Globits.h"
#include "Key.h"
#include "WingedFile.h"
#include "File.h" 

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap
{

public:
	enum Layer {
		LEVEL_LAYER,
		BACKGROUND_LAYER
	};

	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~TileMap();

	void updateEntities(int deltaTime);
	void renderEntities();
	void render(Layer layer) const;
	void renderDynamic(Layer layer);
	void update(int deltaTime);
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }
	glm::ivec2 getPlayerInitPos() const { return tileSize * playerInitPos; }

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int& colTile) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, int& colTile) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int& colTile) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int& colTile) const;
	void breakFragileTiles(const glm::ivec2& pos, const glm::ivec2& size);
	void pickUpKeys(const glm::ivec2& pos, const glm::ivec2& size);
	void startleWingedFiles();
	bool enteredDeathZone(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool enteredSpring(const glm::ivec2& pos, const glm::ivec2& size);
	bool enteredGlobits(const glm::ivec2& pos, const glm::ivec2& size);
	bool enteredFile(const glm::ivec2& pos, const glm::ivec2& size);

private:
	void prepareLayer(Tile** layer, GLuint& vao, GLuint& vbo, const glm::vec2& minCoords, ShaderProgram& program);
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	inline bool tilesOutOfBounds(glm::ivec2 coords) const;

	GLuint vao0, vao1;
	GLuint vbo0, vbo1;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, playerInitPos;
	int tileSize, blockSize, staticTileCountLayer0, staticTileCountLayer1, globitsCount, chestCount, wingedFileCount, fileCount;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	Tile** mapLayer0, ** mapLayer1; // Layer 1 is for background.
	Globits** globits;
	Chest** chests;
	Key** keys;
	WingedFile** wingedFiles;
	File** files;
};


#endif // _TILE_MAP_INCLUDE
