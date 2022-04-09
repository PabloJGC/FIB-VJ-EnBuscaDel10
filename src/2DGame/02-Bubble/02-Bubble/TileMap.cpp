#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	staticTileCountLayer0 = 0;
	staticTileCountLayer1 = 0;
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (mapLayer0 != NULL)
		delete mapLayer0;
	if (mapLayer1 != NULL)
		delete mapLayer1;
	if (globits != NULL)
		delete globits;
}

void TileMap::updateEntities(int deltaTime) {
	for (int i = 0; i < globitsCount; ++i)
		globits[i]->update(deltaTime);
	for (int i = 0; i < chestCount; ++i)
		chests[i]->update(deltaTime);
	for (int i = 0; i < chestCount; ++i)
		keys[i]->update(deltaTime);
}

void TileMap::renderEntities() {
	for (int i = 0; i < globitsCount; ++i)
		globits[i]->render();
	for (int i = 0; i < chestCount; ++i)
		chests[i]->render();
	for (int i = 0; i < chestCount; ++i)
		keys[i]->render();
}

void TileMap::render(Layer layer) const
{
	int staticTileCount;
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	switch (layer) {
		case LEVEL_LAYER:  // Background.
			staticTileCount = staticTileCountLayer0;
			glBindVertexArray(vao0);
			break;
		case BACKGROUND_LAYER:
			staticTileCount = staticTileCountLayer1;
			glBindVertexArray(vao1);
			break;
	}
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * staticTileCount);
	glDisable(GL_TEXTURE_2D);
	glBindVertexArray(0);
}

void TileMap::renderDynamic(Layer layer) {
	switch (layer) {
		case LEVEL_LAYER: {  // Background.
			for (int j = 0; j < mapSize.y; j++) {
				for (int i = 0; i < mapSize.x; i++) {
					mapLayer0[j * mapSize.x + i]->render();
				}
			}
			break;
		}
		case BACKGROUND_LAYER: {
			for (int j = 0; j < mapSize.y; j++) {
				for (int i = 0; i < mapSize.x; i++) {
					mapLayer1[j * mapSize.x + i]->render();
				}
			}
			break;
		}
	}
}

void TileMap::update(int deltaTime) {
	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			mapLayer0[j*mapSize.x + i]->update(deltaTime);
			mapLayer1[j*mapSize.x + i]->update(deltaTime);
		}
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo0);
	glDeleteBuffers(1, &vbo1);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile_id;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> playerInitPos.x >> playerInitPos.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	mapLayer0 = new Tile*[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> tile_id;
			cout << tile_id << endl;
			mapLayer0[j * mapSize.x + i] = Tile::createTile(tile_id, glm::ivec2(i*tileSize, j*tileSize), tileSize);
			staticTileCountLayer0 += !mapLayer0[j * mapSize.x + i]->isDynamic();
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	mapLayer1 = new Tile*[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> tile_id;
			cout << tile_id << endl;
			mapLayer1[j * mapSize.x + i] = Tile::createTile(tile_id, glm::ivec2(i*tileSize, j*tileSize), tileSize);
			staticTileCountLayer1 += !mapLayer1[j * mapSize.x + i]->isDynamic();
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	int entityCount = 0;
	fin >> entityCount;
	for (int i = 0; i < entityCount; ++i) {
		int entityId, count;
		fin >> entityId >> count;
		switch (entityId) {
			case 0: { // GLOBITS
				globitsCount = count;
				globits = new Globits*[count];
				for (int j = 0; j < count; ++j) {
					int x, y;
					fin >> x >> y;
					globits[j] = new Globits(glm::vec2(x*tileSize, y*tileSize));
				}
				break;
			}
			case 2: { // CHEST
				chestCount = count;
				chests = new Chest*[count];
				keys = new Key*[count];
				for (int j = 0; j < count; ++j) {
					int x, y;
					fin >> x >> y;
					chests[j] = new Chest(glm::vec2(x*tileSize, y*tileSize));
					fin >> x >> y;
					keys[j] = new Key(glm::vec2(x*tileSize, y*tileSize), chests[j]);
				}
				break;
			}
		}
	}

	fin.close();

	return true;
	}

void TileMap::prepareLayer(Tile** layer, GLuint& vao, GLuint& vbo, const glm::vec2& minCoords, ShaderProgram& program) {
	Tile* tile;
	int nTiles = 0;
	vector<float> vertices;
	glm::vec2 posTile, texCoordTile[2], halfTexel;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = layer[j * mapSize.x + i];
			int id = tile->getId();
			if (id != 0)
			{
				// Non-empty tile
				if (!tile->isDynamic()) {
					nTiles++;
					posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
					texCoordTile[0] = glm::vec2(float((id - 1) % tilesheetSize.x) / tilesheetSize.x, float((id - 1) / tilesheetSize.x) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					//texCoordTile[0] += halfTexel;
					texCoordTile[1] -= halfTexel;
					// First triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					// Second triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
				}
				else {
					tile->init(program);
				}
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	if (nTiles != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	}
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (int i = 0; i < globitsCount; ++i)
		globits[i]->init(program);
	for (int i = 0; i < chestCount; ++i)
		chests[i]->init(program);
	for (int i = 0; i < chestCount; ++i)
		keys[i]->init(program);
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	prepareLayer(mapLayer0, vao0, vbo0, minCoords, program);
	prepareLayer(mapLayer1, vao1, vbo1, minCoords, program);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

inline bool TileMap::tilesOutOfBounds(glm::ivec2 coords) const {
	return coords.x < 0 || coords.y < 0 || coords.x >= mapSize.x || coords.y >= mapSize.y;
}

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int& colTile) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (!tilesOutOfBounds(glm::ivec2(x, y)) && !mapLayer0[y * mapSize.x + x]->isTransparent()) {
			colTile = x;
			return true;
		}
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int& colTile) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y=y0; y<=y1; y++)
	{
		if (!tilesOutOfBounds(glm::ivec2(x, y)) && !mapLayer0[y * mapSize.x + x]->isTransparent()) {
			colTile = x;
			return true;
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int& colTile) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x=x0; x<=x1; x++)
	{
		if (!tilesOutOfBounds(glm::ivec2(x, y)) && !mapLayer0[y*mapSize.x + x]->isTransparent()) {
			colTile = y;
			//if(*posY - tileSize * y + size.y <= 4)
			//{
				//*posY = tileSize * y - size.y;
			return true;
			//}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int& colTile) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (!tilesOutOfBounds(glm::ivec2(x, y)) && !mapLayer0[y*mapSize.x + x]->isTransparent())
		{
			//if(*posY - tileSize * y + size.y <= 4)
			//{
			//*posY = tileSize * y + size.y;
			colTile = y;
			return true;
			//}
		}
	}

	return false;
}

void TileMap::breakFragileTiles(const glm::ivec2& pos, const glm::ivec2& size) {
	int x0, x1, y0, y1;

	x0 = (pos.x - 1) / tileSize;
	x1 = (pos.x + size.x) / tileSize;
	y0 = (pos.y) / tileSize;
	y1 = (pos.y + size.y) / tileSize;
	for (int x = x0; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {
			if (!tilesOutOfBounds(glm::ivec2(x, y)) && mapLayer0[y * mapSize.x + x]->isFragile()) {
				FragileTile* ft = (FragileTile*)(mapLayer0[y * mapSize.x + x]);
				if (x == x0 || x == x1 || y == y1)
					ft->setBreaking();
				else
					ft->setBlocked();
			}
		}
	}
}

bool TileMap::enteredDeathZone(const glm::ivec2& pos, const glm::ivec2& size) const {
	if (pos.y > mapSize.y*tileSize)
		return true;
	int x0, x1, y0, y1;

	x0 = pos.x/tileSize;
	x1 = (pos.x + size.x - 1)/tileSize;
	y0 = pos.y/tileSize;
	y1 = (pos.y + size.y - 1)/tileSize;
	for (int x = x0; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {
			if (!tilesOutOfBounds(glm::ivec2(x, y)) && mapLayer0[y * mapSize.x + x]->isDeathZone(pos, size)) {
				return true;
			}
		}
	}
	return false;
}

bool TileMap::enteredSpring(const glm::ivec2& pos, const glm::ivec2& size) {
	int x0, x1, y0, y1;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++) {
		for (int y = y0; y <= y1; y++) {
			if (!tilesOutOfBounds(glm::ivec2(x, y)) && mapLayer0[y * mapSize.x + x]->isSpring()) {
				SpringTile* st = (SpringTile*)(mapLayer0[y * mapSize.x + x]);
				st->setPressed();
				return true;
			}
		}
	}
	return false;
}

bool TileMap::enteredGlobits(const glm::ivec2& pos, const glm::ivec2& size) {
	for (int i = 0; i < globitsCount; ++i) {
		if (globits[i]->collides(pos, size))
			return true;
	}
	return false;
}

bool TileMap::enteredZip(const glm::ivec2& pos, const glm::ivec2& size) {
	for (int i = 0; i < chestCount; ++i) {
		if (chests[i]->collides(pos, size))
			return true;
	}
	return false;
}

void TileMap::pickUpKeys(const glm::ivec2& pos, const glm::ivec2& size) {
	for (int i = 0; i < chestCount; ++i) {
		keys[i]->pickUp(pos, size);
	}
}
