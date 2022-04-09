#include "Key.h"

Key::Key(glm::ivec2 pos, Chest* chest) {
	this->pos = pos;
	this->chest = chest;
	spriteSize = glm::ivec2(32, 32);
	colliderSize = glm::ivec2(24, 28);
}

void Key::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 4);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(1.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(2.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(1.f / 16.f, 17.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(0.f / 16.f, 17.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(1.f / 16.f, 17.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(2.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(NORMAL, glm::vec2(1.f / 16.f, 16.f / 32.f));

	sprite->setAnimationSpeed(TAKEN, INT_MAX);
	sprite->addKeyframe(TAKEN, glm::vec2(2.f / 16.f, 17.f / 32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::ivec2(pos));
}

void Key::pickUp(glm::vec2 playerPos, glm::vec2 playerSize) {
	if (state == NORMAL && playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + colliderSize.x &&
		playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + colliderSize.y) {
		chest->unlock();
		state = TAKEN;
		sprite->changeAnimation(TAKEN);
	}
}

void Key::update(int deltaTime) {
	if (state == NORMAL) {
		sprite->update(deltaTime);
	}
}

void Key::render() {
	sprite->render();
}