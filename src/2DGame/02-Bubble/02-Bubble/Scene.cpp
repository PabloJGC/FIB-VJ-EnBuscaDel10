#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 4


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init(int level)
{
	this->level = level;
	initShaders();
	string file;
	switch (level) {
	case 1: {
		file = "levels/level01.txt";
		break;
	}
	case 2: {
		file = "levels/level02.txt";
		break;
	}
	case 3: {
		file = "levels/level03.txt";
		break;
	}
	case 4: {
		file = "levels/level04.txt";
		break;
	}
	case 5: {
		file = "levels/level05.txt";
		break;
	}
	case 6: {
		file = "levels/halfTime.txt";
		break;
	case 7: {
		file = "levels/level06.txt";
		break;
	}
	case 8: {
		file = "levels/level07.txt";
		break;
	}
	case 9: {
		file = "levels/level08.txt";
		break;
	}
	case 10: {
		file = "levels/level09.txt";
		break;
	}
	case 11: {
		file = "levels/level10.txt";
		break;
	}
	case 12: {
		file = "levels/memorial.txt";
		break;
	}
	case 13: {
		file = "levels/victory.txt";
		break;
	}
	}
	}
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	glm::ivec2 playerInitPos = map->getPlayerInitPos();
	player->setPosition(playerInitPos);
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

int Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	map->update(deltaTime);
	map->updateEntities(deltaTime);
	bool nextLevel = player->update(deltaTime);
	return level + int(nextLevel);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	// Layers rendered behind the player:
	map->render(TileMap::BACKGROUND_LAYER);
	map->renderDynamic(TileMap::BACKGROUND_LAYER);
	map->renderEntities();
	map->render(TileMap::LEVEL_LAYER);
	map->renderDynamic(TileMap::LEVEL_LAYER);

	// Player:
	player->render();
	// Layers rendered in front of the player:
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
