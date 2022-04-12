#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0


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
		player = new Player();
		break;
	}
	case 2: {
		file = "levels/level02.txt";
		player = new Player();
		break;
	}
	case 3: {
		file = "levels/level03.txt";
		player = new Player();
		break;
	}
	case 4: {
		file = "levels/level04.txt";
		player = new Player();
		break;
	}
	case 5: {
		file = "levels/level05.txt";
		player = new Player();
		break;
	}
	case 6: {
		file = "levels/halfTime.txt";
		player = new PlayerPablo();
		break;
	}
	case 7: {
		file = "levels/level06.txt";
		player = new PlayerPablo();
		break;
	}
	case 8: {
		file = "levels/level07.txt";
		player = new PlayerPablo();
		break;
	}
	case 9: {
		file = "levels/level08.txt";
		player = new PlayerPablo();
		break;
	}
	case 10: {
		file = "levels/level09.txt";
		player = new PlayerPablo();
		break;
	}
	case 11: {
		file = "levels/level10.txt";
		player = new PlayerPablo();
		break;
	}
	case 12: {
		file = "levels/memorial.txt";
		player = new PlayerPablo();
		break;
	}
	case 13: {
		file = "levels/victory.txt";
		player = new PlayerPablo();
		break;
	}
	}
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
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
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end();) {
		(*it)->update(deltaTime);
		if ((*it)->exceededMaxLifeTime()) {
			(*it)->free();
			delete (*it);
			it = particles.erase(it);
		}
		else
			++it;
	}
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
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
		(*it)->render();
}

void Scene::addParticle(Particle* particle) {
	particles.push_back(particle);
	particle->init(texProgram);
}

void Scene::generateDustParticle(glm::ivec2 pos) {
	DustParticle* dp = new DustParticle(pos);
	addParticle(dp);
}

void Scene::generateExplosionParticle(glm::ivec2 pos) {
	ExplosionParticle* ep0 = new ExplosionParticle(pos, 45, 0);
	addParticle(ep0);
	ExplosionParticle* ep1 = new ExplosionParticle(pos, 120, 1);
	addParticle(ep1);
	ExplosionParticle* ep2 = new ExplosionParticle(pos, 225, 2);
	addParticle(ep2);
	ExplosionParticle* ep3 = new ExplosionParticle(pos, 305, 3);
	addParticle(ep3);
	ExplosionParticle* ep4 = new ExplosionParticle(pos, 350, 4);
	addParticle(ep4);
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
