#include "MenuScene.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "GameScene.h"
#include "Game.h"

#define FLASH_TIME 400

MenuScene::MenuScene() {}

MenuScene::~MenuScene() {}

void MenuScene::init(int level) {
	Scene::init(level);
	Game::instance().setBackgroundColor(0, 0, 0);
	glm::vec2 titleSize = glm::vec2(32.f*6.f + 3.f, 64.f)*2.f;
	glm::vec2 titlePos = glm::vec2(SCREEN_WIDTH/2 - titleSize.x/2.f, 32.f);
	glm::vec2 geom[2] = { titlePos, titlePos + titleSize };
	glm::vec2 texCoords[2] = { glm::vec2((10.f - 3.f/8.f)/16.f, 30.f/32.f), glm::vec2(1.f, 1.f) };
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	textures.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textures.setMagFilter(GL_NEAREST);
}

int MenuScene::update(int deltaTime) {
	Scene::update(deltaTime);
	if (Game::instance().getJumpKeyPressed())
		return level + 1;
}

void MenuScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
		(*it)->render();

	texQuad->render(textures);

	text.render("C: SALTAR", glm::vec2(32*5.5, 32*7), 32, glm::vec4(1, 1, 1, 1));
	text.render("X: DASH", glm::vec2(32*6, 32*8), 32, glm::vec4(1, 1, 1, 1));
	if (int(currentTime/FLASH_TIME)%2 == 0)
		text.render("PULSA C PARA EMPEZAR", glm::vec2(32*1.5, 32*9), 32, glm::vec4(1, 1, 1, 1));
	text.render("CODIGO:", glm::vec2(32*6.5, 32*11), 32, glm::vec4(0.5, 0.5, 0.5, 1));
	text.render("PABLO GALVAN", glm::vec2(32*4.5, 32*12), 32, glm::vec4(0.5, 0.5, 0.5, 1));
	text.render("AFTAB AHMED", glm::vec2(32*4.5, 32*13), 32, glm::vec4(0.5, 0.5, 0.5, 1));
	text.render("ARTE:", glm::vec2(32*7, 32*14), 32, glm::vec4(0.5, 0.5, 0.5, 1));
	text.render("AN GALERA", glm::vec2(32*5.5, 32*15), 32, glm::vec4(0.5, 0.5, 0.5, 1));
	text.render("AN GALERA", glm::vec2(32*5.5, 32*15), 32, glm::vec4(0.5, 0.5, 0.5, 1));
}
