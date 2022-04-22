#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0
#define LEVEL_DISPLAY_TIME 1500.f
#define SNOW_GEN_TIME 130

Scene::Scene(){
}

Scene::~Scene() {
}

void Scene::init(int level)
{
	particles.clear();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	this->level = level;
	currentTime = 0;
	snowTimer = 0;
	initShaders();
	if (!text.init("fonts/Pixeled.ttf"))
		cout << "Could not load font!!!" << endl;
}

int Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	snowTimer += deltaTime;
	if (snowTimer >= SNOW_GEN_TIME) {
		snowTimer = 0;
		generateSnowParticle(0, SCREEN_HEIGHT - 1, currentTime);
	}
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
	return level;
}

void Scene::setCameraOffset(int x, int y) {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1 + x), float(SCREEN_HEIGHT - 1 + y), 0.f);
}
void Scene::generateSnowParticle(int minY, int maxY, int time) {
	SnowParticle* dp = new SnowParticle(minY, maxY, time);
	addParticle(dp);
}

void Scene::addParticle(Particle* particle) {
	particles.push_back(particle);
	particle->init(texProgram);
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
