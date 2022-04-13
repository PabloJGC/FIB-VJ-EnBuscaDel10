#include "FragileTile.h"
#include "Game.h"

#define BREAKING_TIME 150
#define REGENERATION_TIME 3000

FragileTile::FragileTile(int id, glm::ivec2 pos, int size) : Tile(id, pos, size) {
	timer = 0;
	spriteSize = glm::ivec2(size, size);
}

void FragileTile::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8./128., 8./256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(NORMAL, INT_MAX);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 19.f/32.f));

	sprite->setAnimationSpeed(BREAKING, 1000/BREAKING_TIME);
	sprite->addKeyframe(BREAKING, glm::vec2(0.f, 19.f/32.f));
	sprite->addKeyframe(BREAKING, glm::vec2(1.f/16.f, 19.f/32.f));
	sprite->addKeyframe(BREAKING, glm::vec2(2.f/16.f, 19.f/32.f));

	sprite->setAnimationSpeed(BROKEN, INT_MAX);
	sprite->addKeyframe(BROKEN, glm::vec2(3.f/16.f, 19.f/32.f));

	state = NORMAL;
	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void FragileTile::update(int deltaTime) {
	switch (state) {
		case NORMAL: {
			sprite->changeAnimation(NORMAL);
			transparent = false;
			break;
		}
		case BREAKING: {
			timer += deltaTime;
			if (timer >= BREAKING_TIME*3) {
				timer = 0;
				state = BROKEN;
				transparent = true;
				sprite->changeAnimation(BROKEN);
			}
			else
				sprite->update(deltaTime);
			break;
		}
		case BROKEN: {
			timer += deltaTime;
			if (timer >= REGENERATION_TIME) {
				Game::instance().getScene()->generateDustParticle(pos);
				state = NORMAL;
				transparent = false;
				sprite->changeAnimation(NORMAL);
			}
			break;
		}
		case BLOCKED: {
			state = NORMAL;
			break;
		}
	}
}

void FragileTile::render() const {
	sprite->render();
}

void FragileTile::setBlocked() {
	if (state == NORMAL) {
		state = BLOCKED;
		transparent = true;
		sprite->changeAnimation(BROKEN);
	}
}

void FragileTile::setBreaking() {
	if (state == NORMAL) {
		timer = 0;
		state = BREAKING;
		sprite->changeAnimation(BREAKING);
	}
}