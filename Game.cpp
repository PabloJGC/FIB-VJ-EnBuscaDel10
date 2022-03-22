#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	currentLevel = 1;
	bPlay = true;
	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	scene.init(1);
}

bool Game::update(int deltaTime)
{
	int level = scene.update(deltaTime);
	if (level != currentLevel) {
		currentLevel = level;
		scene.init(currentLevel);
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





