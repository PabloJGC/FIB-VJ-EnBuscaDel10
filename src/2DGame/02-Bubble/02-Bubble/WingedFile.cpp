#include "WingedFile.h"

#define FLOAT_HEIGHT 8
#define FLOAT_PERIOD 500
#define FLEE_ACCELERATION 1.f


WingedFile::WingedFile(glm::ivec2 pos) {
	this->pos = pos;
	initPos = pos.y;
	spriteSize = glm::ivec2(32*3, 32);
	colliderSize = glm::ivec2(32, 32);
	colliderOffset = glm::ivec2(32, 0);
}

void WingedFile::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(24. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(STARTLED, 16);
	sprite->addKeyframe(STARTLED, glm::vec2(0.f, 20.f / 32.f));
	sprite->addKeyframe(STARTLED, glm::vec2(0.f, 21.f / 32.f));
	sprite->addKeyframe(STARTLED, glm::vec2(0.f, 22.f / 32.f));
	sprite->addKeyframe(STARTLED, glm::vec2(0.f, 21.f / 32.f));


	sprite->setAnimationSpeed(NORMAL, 4);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 20.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 21.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 22.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 21.f / 32.f));

	sprite->setAnimationSpeed(TAKEN, INT_MAX);
	sprite->addKeyframe(TAKEN, glm::vec2(1.f / 16.f, 15.f / 32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition((glm::ivec2(pos)/4)*4);
}

bool WingedFile::collides(glm::vec2 playerPos, glm::vec2 playerSize) {
	if (state == TAKEN)
		return false;
	if (playerPos.x + playerSize.x >= pos.x + colliderOffset.x && playerPos.x <= pos.x + colliderSize.x + colliderOffset.x &&
		playerPos.y + playerSize.y >= pos.y + colliderOffset.y && playerPos.y <= pos.y + colliderSize.y + colliderOffset.y) {
		state = TAKEN;
		sprite->changeAnimation(TAKEN);
		return true;
	}
	return false;
}

void WingedFile::startle() {
	if (state == NORMAL) {
		timer = 0;
		state = STARTLED;
		sprite->changeAnimation(STARTLED);
	}
}

void WingedFile::update(int deltaTime) {
	switch (state) {
		case NORMAL: {
			timer += deltaTime;
			pos.y = initPos + FLOAT_HEIGHT*sin(3.14159f*float(timer)/FLOAT_PERIOD);
			sprite->setPosition((glm::ivec2(pos)/4)*4);
			sprite->update(deltaTime);
			break;
		}
		case STARTLED: {
			timer += deltaTime;
			float t = float(timer)/50.f;
			pos.y = initPos - FLEE_ACCELERATION*t*t;
			sprite->setPosition((glm::ivec2(pos)/4)*4);
			sprite->update(deltaTime);
			break;
		}
	}
}

void WingedFile::render() {
	sprite->render();
}