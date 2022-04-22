#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define MAX_LEVEL 13


void Game::init()
{
	engine = createIrrKlangDevice();
	engine->setSoundVolume(0.3f);
	scene = new MenuScene();
	currentLevel = 0;
	bPlay = true;

	godMode = false;
	slowMode = false;
	infiniteDashMode = false;

	scene->init(currentLevel);
}

void Game::updateMode() {
	if (getGodModeKeyPressed()) {
		godMode = !godMode;
	}
	if (getSlowModeKeyPressed()) {
		slowMode = !slowMode;
	}
	if (getInfiniteDashModeKeyPressed()) {
		infiniteDashMode = !infiniteDashMode;
	}
}

bool Game::update(int deltaTime)
{
	updateMode();

	int newDeltaTime = deltaTime;
	if (slowMode) newDeltaTime /= 2;
	int level = scene->update(newDeltaTime);
	for (int i = 0; i < 256; ++i) {
		if (keyStatus[i] == PRESSED)
			keyStatus[i] = DOWN;
	}
	if (level != currentLevel) {
		if (currentLevel == 0) {
			delete scene;
			scene = new GameScene();
		}
		
		if (currentLevel < MAX_LEVEL) {
			currentLevel = level;
			scene->init(currentLevel);
		}
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->render();
}

void Game::setBackgroundColor(float red, float green, float blue) {
	glClearColor(red, green, blue, 1.0f);
}

void Game::increaseScore() {
	++score;
}

void Game::increaseDeathCount() {
	++deathCount;
}

void Game::playSound(char* file) {
	if (!engine)
		return;
	engine->play2D(file, false);
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	else if (key >= 48 && key <= 57) {
		currentLevel = key - 47;
		scene->init(currentLevel);
	}
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

bool Game::getGodModeKeyPressed() {
	return keyStatus[GOD_MODE_KEY_UC] == PRESSED || keyStatus[GOD_MODE_KEY_LC] == PRESSED;
}

bool Game::getSlowModeKeyPressed() {
	return keyStatus[SLOW_MODE_KEY_UC] == PRESSED || keyStatus[SLOW_MODE_KEY_LC] == PRESSED;
}

bool Game::getInfiniteDashModeKeyPressed() {
	return keyStatus[INFINITE_DASH_MODE_KEY_UC] == PRESSED || keyStatus[INFINITE_DASH_MODE_KEY_LC] == PRESSED;
}

bool Game::getGodMode() {
	return godMode;
}

bool Game::getSlowMode() {
	return slowMode;
}

bool Game::getInfiniteDashMode() {
	return infiniteDashMode;
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

Scene* Game::getScene() {
	return scene;
}
