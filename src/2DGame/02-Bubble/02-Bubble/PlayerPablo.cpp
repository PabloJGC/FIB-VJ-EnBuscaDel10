#include "PlayerPablo.h"

void PlayerPablo::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	state = NORMAL;

	spriteSize = glm::vec2(32, 32);
	hitboxOffset = glm::vec2(4, 0);
	hitboxSize = glm::vec2(24, 32);

	canDash = true;
	canClimb = false;
	dead = false;
	dashDirection = glm::ivec2(0);
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(20);

	sprite->setAnimationSpeed(STAND_LEFT, 10);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(4.f / 16.f, 19.f / 32.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 10);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(4.f / 16.f, 16.f / 32.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(5.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(6.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4.f / 16.f, 20.f / 32.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4.f / 16.f, 17.f / 32.f));

	sprite->setAnimationSpeed(JUMP_LEFT, 10);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(4.f / 16.f, 20.f / 32.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 10);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(4.f / 16.f, 17.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_LEFT, 10);
	sprite->addKeyframe(LOOK_UP_LEFT, glm::vec2(5.f / 16.f, 21.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_RIGHT, 10);
	sprite->addKeyframe(LOOK_UP_RIGHT, glm::vec2(5.f / 16.f, 18.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_LEFT, 10);
	sprite->addKeyframe(LOOK_DOWN_LEFT, glm::vec2(4.f / 16.f, 21.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_RIGHT, 10);
	sprite->addKeyframe(LOOK_DOWN_RIGHT, glm::vec2(4.f / 16.f, 18.f / 32.f));


	sprite->setAnimationSpeed(STAND_LEFT_NO_DASH, 10);
	sprite->addKeyframe(STAND_LEFT_NO_DASH, glm::vec2(4.f / 16.f, 25.f / 32.f));

	sprite->setAnimationSpeed(STAND_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(STAND_RIGHT_NO_DASH, glm::vec2(4.f / 16.f, 22.f / 32.f));

	sprite->setAnimationSpeed(MOVE_LEFT_NO_DASH, 10);
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(4.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(5.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(6.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(4.f / 16.f, 26.f / 32.f));

	sprite->setAnimationSpeed(MOVE_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(4.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(5.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(6.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(4.f / 16.f, 23.f / 32.f));

	sprite->setAnimationSpeed(JUMP_LEFT_NO_DASH, 10);
	sprite->addKeyframe(JUMP_LEFT_NO_DASH, glm::vec2(4.f / 16.f, 26.f / 32.f));

	sprite->setAnimationSpeed(JUMP_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(JUMP_RIGHT_NO_DASH, glm::vec2(4.f / 16.f, 23.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_LEFT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_UP_LEFT_NO_DASH, glm::vec2(5.f / 16.f, 27.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_UP_RIGHT_NO_DASH, glm::vec2(5.f / 16.f, 24.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_LEFT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_DOWN_LEFT_NO_DASH, glm::vec2(4.f / 16.f, 27.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_DOWN_RIGHT_NO_DASH, glm::vec2(4.f / 16.f, 24.f / 32.f));

	sprite->changeAnimation(STAND_RIGHT);
	facingDirection = RIGHT;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}