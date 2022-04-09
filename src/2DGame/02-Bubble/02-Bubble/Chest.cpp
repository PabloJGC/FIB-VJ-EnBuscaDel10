#include "Chest.h"

#define FLOAT_HEIGHT 8
#define FLOAT_PERIOD 500

Chest::Chest(glm::ivec2 pos) {
	this->pos = pos;
	initPos = pos.y;
	spriteSize = glm::ivec2(32, 32);
	colliderSize = glm::ivec2(32, 32);
}

void Chest::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(LOCKED, INT_MAX);
	sprite->addKeyframe(LOCKED, glm::vec2(3.f / 16.f, 16.f / 32.f));

	sprite->setAnimationSpeed(NORMAL, INT_MAX);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f / 16.f, 15.f / 32.f));

	sprite->setAnimationSpeed(TAKEN, INT_MAX);
	sprite->addKeyframe(TAKEN, glm::vec2(3.f / 16.f, 17.f / 32.f));

	state = LOCKED;
	sprite->changeAnimation(LOCKED);
	sprite->setPosition((glm::ivec2(pos)/4)*4);
}

bool Chest::collides(glm::vec2 playerPos, glm::vec2 playerSize) {
	if (state == LOCKED || state == TAKEN)
		return false;
	if (playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + colliderSize.x &&
		playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + colliderSize.y) {
		state = TAKEN;
		sprite->changeAnimation(TAKEN);
		return true;
	}
	return false;
}

void Chest::unlock() {
	state = NORMAL;
	sprite->changeAnimation(NORMAL);
}

void Chest::update(int deltaTime) {
	if (state == NORMAL) {
		timer += deltaTime;
		pos.y = initPos + FLOAT_HEIGHT*sin(3.14159f*float(timer)/FLOAT_PERIOD);
		sprite->setPosition((glm::ivec2(pos)/4)*4);
	}
}

void Chest::render() {
	sprite->render();
}