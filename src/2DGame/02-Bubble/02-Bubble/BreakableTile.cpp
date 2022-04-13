#include "BreakableTile.h"
#include "Game.h"

BreakableTile::BreakableTile(int id, glm::ivec2 pos, int size) : Tile(id, pos, size) {
	spriteSize = glm::ivec2(size, size);
}

void BreakableTile::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, INT_MAX);
	sprite->addKeyframe(NORMAL, glm::vec2(float((id - 1)%16)/16.f, float((id - 1)/16)/32.f));

	sprite->setAnimationSpeed(BROKEN, INT_MAX);
	sprite->addKeyframe(BROKEN, glm::vec2(3.f / 16.f, 19.f / 32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void BreakableTile::render() const {
	sprite->render();
}

void BreakableTile::setBroken() {
	if (state == NORMAL) {
		Game::instance().getScene()->generateDustParticle(pos);
		transparent = true;
		state = BROKEN;
		sprite->changeAnimation(BROKEN);
	}
}