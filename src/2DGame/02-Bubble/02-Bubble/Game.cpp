#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	currentLevel = 7;
	bPlay = true;
	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	scene.init(currentLevel);
}

bool Game::update(int deltaTime)
{
	int level = scene.update(deltaTime);
	for (int i = 0; i < 256; ++i) {
		if (keyStatus[i] == PRESSED)
			keyStatus[i] = DOWN;
	}
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
	if (keyStatus[key] == UP)
		keyStatus[key] = PRESSED;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	keyStatus[key] = UP;
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

bool Game::getJumpKeyPressed() {
	return keyStatus[JUMP_KEY_UC] == PRESSED || keyStatus[JUMP_KEY_LC] == PRESSED;
}

bool Game::getDashKeyPressed() {
	return keyStatus[DASH_KEY_UC] == PRESSED || keyStatus[DASH_KEY_LC] == PRESSED;
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





