#include "Cloud.h"

#define CLOUD_SPEED 1.f

Cloud::Cloud(glm::ivec2 pos, bool moveL) {
	this->pos = pos;
	initPos = pos.x;
	moveLeft = moveL;
	spriteSize = glm::ivec2(32 * 2, 32);
	colliderSize = glm::ivec2(32 * 2, 4);
}

void Cloud::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(16. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0 ,glm::vec2(0.f, 18.f / 32.f));

	sprite->changeAnimation(0);
	sprite->setPosition((glm::ivec2(pos) / 4) * 4);
}

bool Cloud::collides(glm::vec2 playerPos, glm::vec2 playerSize) {
	return false;
}

void Cloud::update(int deltaTime, glm::ivec2 mapSize, int tileSize) {
	timer += deltaTime;
	if (moveLeft) {
		if (pos.x > -2 * tileSize) {
			pos.x -= CLOUD_SPEED;
		}
		else {
			pos.x = mapSize.y * tileSize;
		}
	}
	else {
		if (pos.x < mapSize.y * tileSize) {
			pos.x += CLOUD_SPEED;
		}
		else {
			pos.x = -2 * tileSize;
		}
	}
	sprite->setPosition((glm::ivec2(pos) / 4) * 4);
	sprite->update(deltaTime);
}

void Cloud::render() {
	sprite->render();
}
