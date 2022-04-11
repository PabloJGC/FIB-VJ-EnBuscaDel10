#include "File.h"

#define FLOAT_HEIGHT 8
#define FLOAT_PERIOD 500

File::File(glm::ivec2 pos) {
	this->pos = pos;
	initPos = pos.y;
	spriteSize = glm::ivec2(32, 32);
	colliderSize = glm::ivec2(32, 32);
}

void File::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 4);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 15.f/32.f));

	sprite->setAnimationSpeed(TAKEN, INT_MAX);
	sprite->addKeyframe(TAKEN, glm::vec2(2.f/16.f, 17.f/32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition((glm::ivec2(pos) / 4) * 4);
}

bool File::collides(glm::vec2 playerPos, glm::vec2 playerSize) {
	if (state == TAKEN)
		return false;
	if (playerPos.x + playerSize.x >= pos.x && playerPos.x <= pos.x + colliderSize.x &&
		playerPos.y + playerSize.y >= pos.y && playerPos.y <= pos.y + colliderSize.y) {
		timer = 0;
		state = TAKEN;
		sprite->changeAnimation(TAKEN);
		return true;
	}
	return false;
}

void File::update(int deltaTime) {
	timer += deltaTime;
	if (state == NORMAL) {
		pos.y = initPos + FLOAT_HEIGHT * sin(3.14159f * float(timer) / FLOAT_PERIOD);
		sprite->setPosition((glm::ivec2(pos) / 4) * 4);
		sprite->update(deltaTime);
	}
}

void File::render() {
	sprite->render();
}
