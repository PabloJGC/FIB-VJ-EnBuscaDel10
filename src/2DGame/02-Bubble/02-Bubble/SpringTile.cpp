#include "SpringTile.h"

#define PRESSED_TIME 100

SpringTile::SpringTile(int id, glm::ivec2 pos, int size) : Tile(id, pos, size) {
	timer = 0;
	spriteSize = glm::ivec2(32, 32);
}

void SpringTile::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8./128., 8./256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, INT_MAX);
	sprite->addKeyframe(NORMAL, glm::vec2(0, 9.f/32.f));

	sprite->setAnimationSpeed(PRESSED, INT_MAX);
	sprite->addKeyframe(PRESSED, glm::vec2(1.f/16.f, 9.f/32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void SpringTile::update(int deltaTime) {
	if (state == PRESSED) {
		timer += deltaTime;
		if (timer >= PRESSED_TIME) {
			state = NORMAL;
			sprite->changeAnimation(NORMAL);
		}
	}
	//sprite->update(deltaTime);
}

void SpringTile::render() const {
	sprite->render();
}

void SpringTile::setPressed() {
	state = PRESSED;
	sprite->changeAnimation(PRESSED);
	timer = 0;
}