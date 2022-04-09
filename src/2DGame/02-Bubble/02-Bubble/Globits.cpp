#include "Globits.h"

#define FLOAT_HEIGHT 8
#define FLOAT_PERIOD 1000
#define REGENERATION_TIME 2500

Globits::Globits(glm::ivec2 pos) {
	this->pos = pos;
	initPos = pos.y;
	spriteSize = glm::ivec2(24, 60);
	colliderSize = glm::ivec2(24, 28);
}

void Globits::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(6./128., 15./256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 3);
	sprite->addKeyframe(NORMAL, glm::vec2(7.f/16.f, 56.f/128.f));
	sprite->addKeyframe(NORMAL, glm::vec2(8.f/16.f, 56.f/128.f));
	sprite->addKeyframe(NORMAL, glm::vec2(9.f/16.f, 56.f/128.f));
	sprite->addKeyframe(NORMAL, glm::vec2(8.f/16.f, 56.f/128.f));

	sprite->setAnimationSpeed(BLOWN_UP, INT_MAX);
	sprite->addKeyframe(BLOWN_UP, glm::vec2(10.f/16.f, 56.f/128.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition((glm::ivec2(pos)/4)*4);
}

bool Globits::collides(glm::vec2 playerPos, glm::vec2 playerSize) {
	if (state == BLOWN_UP)
		return false;
	if (playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + colliderSize.x &&
		playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + colliderSize.y) {
		timer = 0;
		state = BLOWN_UP;
		sprite->changeAnimation(BLOWN_UP);
		return true;
	}
	return false;
}

void Globits::update(int deltaTime) {
	timer += deltaTime;
	if (state == NORMAL) {
		pos.y = initPos + FLOAT_HEIGHT*sin(3.14159f*float(timer)/FLOAT_PERIOD);
		sprite->setPosition((glm::ivec2(pos) / 4) * 4);
		sprite->update(deltaTime);
	}
	else if (timer >= REGENERATION_TIME) {
		state = NORMAL;
		sprite->changeAnimation(NORMAL);
	}
}

void Globits::render() {
	sprite->render();
}