#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0
#define LEVEL_DISPLAY_TIME 1500.f


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
	if (levelTextBg != NULL)
		delete levelTextBg;
}


void Scene::init(int level)
{
	this->level = level;
	initShaders();
	string file;
	doShowResults = false;
	switch (level) {
		case 1: {
			levelName = "A5S08";
			file = "levels/level01.txt";
			player = new Player();
			break;
		}
		case 2: {
			levelName = "A5001";
			file = "levels/level02.txt";
			player = new Player();
			break;
		}
		case 3: {
			levelName = "A5102";
			file = "levels/level03.txt";
			player = new Player();
			break;
		}
		case 4: {
			levelName = "A5103";
			file = "levels/level04.txt";
			player = new Player();
			break;
		}
		case 5: {
			levelName = "A5202";
			file = "levels/level05.txt";
			player = new Player();
			break;
		}
		case 6: {
			levelName = " DOOR";
			file = "levels/halfTime.txt";
			player = new PlayerPablo();
			break;
		}
		case 7: {
			levelName = "A6001";
			file = "levels/level06.txt";
			player = new PlayerPablo();
			break;
		}
		case 8: {
			levelName = "A6101";
			file = "levels/level07.txt";
			player = new PlayerPablo();
			break;
		}
		case 9: {
			levelName = "A6104";
			file = "levels/level08.txt";
			player = new PlayerPablo();
			break;
		}
		case 10: {
			levelName = "A6203";
			file = "levels/level09.txt";
			player = new PlayerPablo();
			break;
		}
		case 11: {
			levelName = "A6204";
			file = "levels/level10.txt";
			player = new PlayerPablo();
			break;
		}
		case 12: {
			levelName = " BAR ";
			file = "levels/memorial.txt";
			player = new PlayerPablo();
			break;
		}
		case 13: {
			doShowResults = true;
			levelName = "Omega";
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
	if (!text.init("fonts/Pixeled.ttf"))
		cout << "Could not load font!!!" << endl;
	levelTextBg = Quad::createQuad(32*5 - 4, 32*7, 32*6 + 8, 32 + 4, simpleProgram);
	resultsTextBg = Quad::createQuad(32*2 - 4, 32, 32*12 + 8, 32*4 + 4, simpleProgram);
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

	if (currentTime <= LEVEL_DISPLAY_TIME) {
		simpleProgram.use();
		simpleProgram.setUniformMatrix4f("modelview", modelview);
		simpleProgram.setUniformMatrix4f("projection", projection);
		simpleProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
		levelTextBg->render();
		simpleProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		text.render(levelName, glm::vec2(16 * 13, 32 * 8), 32, glm::vec4(1, 1, 1, 1));
	}

	if (doShowResults) {
		simpleProgram.use();
		simpleProgram.setUniformMatrix4f("modelview", modelview);
		simpleProgram.setUniformMatrix4f("projection", projection);
		simpleProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
		resultsTextBg->render();
		simpleProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		text.render("iFelicidades!", glm::vec2(32*4, 32*2), 32, glm::vec4(1, 1, 1, 1));
		text.render("Has sacado un 10.", glm::vec2(32*3, 32*3), 32, glm::vec4(1, 1, 1, 1));
		char buffer[32];
		sprintf(buffer, "Apuntes: %d", Game::instance().getScore());
		text.render(buffer, glm::vec2(32*5, 32*4), 32, glm::vec4(1, 1, 1, 1));
		sprintf(buffer, "Muertes: %d", Game::instance().getDeathCount());
		text.render(buffer, glm::vec2(32*5, 32*5), 32, glm::vec4(1, 1, 1, 1));
	}
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

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

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
