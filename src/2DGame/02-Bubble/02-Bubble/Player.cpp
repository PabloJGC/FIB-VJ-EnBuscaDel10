#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
#define RUN_MAX_SPEED 0.25f
#define FALL_SPEED 0.25f
#define CLIMB_FALL_SPEED 0.05f
#define JUMP_MAX_SPEED 0.6f
#define DASH_TIME 200
#define DASH_MAX_SPEED 0.75f
#define DASH_MIN_SPEED 0.5f
#define WALL_JUMP_TIME 190
#define WALL_JUMP_MAX_SPEED 0.7f
#define WALL_JUMP_MIN_SPEED 0.5f
#define WALL_JUMP_DISTANCE 3
#define PIXEL_SIZE 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
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

	sprite->setNumberAnimations(4);

	// TODO: añadir opción de invertir los keyframes horizontalmente.
	sprite->setAnimationSpeed(STAND_LEFT, 10);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.4375f, 16.f / 32.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 10);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.4375f, 16.f / 32.f));

	// TODO: añadir opción de invertir los keyframes horizontalmente.
	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4375f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(8.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5625f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4375f, 0.53125f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4375f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5625f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4375f, 0.53125f));

	sprite->changeAnimation(STAND_RIGHT);
	facingDirection = RIGHT;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

bool Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	updateState(deltaTime);

	switch (state) {
	case NORMAL: {
		move();
		velocity.y = FALL_SPEED;
		break;
	}
	case JUMPING: {
		move();
		velocity.y = min(FALL_SPEED, -JUMP_MAX_SPEED * cos(3.14159f * jumpAngle / 180.f));
		break;
	}
	case DASHING: {
		velocity = dashDirection * (DASH_MIN_SPEED + (DASH_MAX_SPEED - DASH_MIN_SPEED) * (dashTimer / DASH_TIME));
		break;
	}
	case CLIMBING: {
		move();
		velocity.y = CLIMB_FALL_SPEED;
		break;
	}
	case WALL_JUMPING: {
		velocity = dashDirection * (WALL_JUMP_MIN_SPEED + (WALL_JUMP_MAX_SPEED - WALL_JUMP_MIN_SPEED) * (dashTimer / WALL_JUMP_TIME));
		break;
	}
	}

	updatePosition(deltaTime);
	dead = posPlayer.y > (map->getMapSize().y * map->getTileSize());
	if (dead) {
		posPlayer = map->getPlayerInitPos();
		canDash = true;
		canClimb = false;
		dead = false;
		state = NORMAL;
		facingDirection = RIGHT;
		sprite->changeAnimation(STAND_RIGHT);
	}
	return posPlayer.y < 0;
}

void Player::updateState(int deltaTime) {
	grounded = map->collisionMoveDown(glm::ivec2(posPlayer) + hitboxOffset + glm::ivec2(0, 1), hitboxSize);
	if (grounded && state != DASHING)
		canDash = true;
	canClimb = !grounded && velocity.y > 0;

	switch (state) {
	case NORMAL: {
		if (Game::instance().getJumpKeyPressed()) {
			if (grounded) {
				state = JUMPING;
				jumpAngle = 0;
			}
			else if (wallAt(RIGHT, WALL_JUMP_DISTANCE)) {
				wallJump(RIGHT);
			}
			else if (wallAt(LEFT, WALL_JUMP_DISTANCE)) {
				wallJump(LEFT);
			}
		}
		if (state == NORMAL) {
			if (canDash && Game::instance().getDashKeyPressed()) {
				dash();
			}
			else if (canClimb && ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && map->collisionMoveRight(glm::ivec2(posPlayer + glm::vec2(1.f, 0.f)) + hitboxOffset, hitboxSize)) ||
				(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && map->collisionMoveLeft(glm::ivec2(posPlayer + glm::vec2(-1.f, 0.f)) + hitboxOffset, hitboxSize)))) {
				state = CLIMBING;
			}
		}
		break;
	}
	case JUMPING: {
		jumpAngle += JUMP_ANGLE_STEP;
		if (Game::instance().getJumpKeyPressed()) {
			if (wallAt(RIGHT, WALL_JUMP_DISTANCE)) {
				wallJump(RIGHT);
			}
			else if (wallAt(LEFT, WALL_JUMP_DISTANCE)) {
				wallJump(LEFT);
			}
		}
		if (state == JUMPING) {
			if (canDash && Game::instance().getDashKeyPressed()) {
				dash();
			}
			else if (jumpAngle >= 180 ||
				(jumpAngle > 90 && grounded))
				state = NORMAL;
			else if (canClimb && ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && wallAt(RIGHT, 1)) ||
				(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && wallAt(LEFT, 1)))) {
				state = CLIMBING;
			}
		}
		break;
	}
	case DASHING: {
		dashTimer -= deltaTime;
		if (dashTimer <= 0) {
			if (dashDirection.y > 0)
				state = NORMAL;
			else if (dashDirection.y == 0) {
				state = JUMPING;
				jumpAngle = 90;
			}
			else {
				state = JUMPING;
				jumpAngle = 60;
			}
		}
		break;
	}
	case CLIMBING: {
		if (Game::instance().getJumpKeyPressed()) {
			if (wallAt(RIGHT, WALL_JUMP_DISTANCE)) {
				wallJump(RIGHT);
			}
			else if (wallAt(LEFT, WALL_JUMP_DISTANCE)) {
				wallJump(LEFT);
			}
		}
		if (state == CLIMBING) {
			if (canDash && Game::instance().getDashKeyPressed()) {
				dash();
			}
			else if (grounded || !((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && wallAt(RIGHT, 1)) ||
				(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && wallAt(LEFT, 1)))) {
				state = NORMAL;
			}
		}
		break;
	}
	case WALL_JUMPING: {
		if (canDash && Game::instance().getDashKeyPressed()) {
			dash();
		}
		else {
			dashTimer -= deltaTime;
			if (dashTimer <= 0) {
				state = JUMPING;
				jumpAngle = 60;
			}
		}
		break;
	}
	}
}

bool Player::wallAt(FacingDirection direction, int offset) const {
	if (direction == RIGHT)
		return map->collisionMoveRight(glm::ivec2(posPlayer + glm::vec2(offset, 0)) + hitboxOffset, hitboxSize);
	return map->collisionMoveLeft(glm::ivec2(posPlayer + glm::vec2(-offset, 0)) + hitboxOffset, hitboxSize);
}

void Player::wallJump(FacingDirection facingDirection) {
	dashDirection.y = -1;
	dashDirection.x = facingDirection == LEFT ? 1 : -1;

	dashDirection = glm::normalize(dashDirection);

	dashTimer = WALL_JUMP_TIME;
	state = WALL_JUMPING;
}

void Player::dash() {
	dashDirection.y = Game::instance().getSpecialKey(GLUT_KEY_UP) ? -1
		: Game::instance().getSpecialKey(GLUT_KEY_DOWN) ? 1
		: 0;
	if (dashDirection.y != 0)
		dashDirection.x = Game::instance().getSpecialKey(GLUT_KEY_LEFT) ? -1
		: Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ? 1
		: 0;
	else
		dashDirection.x = facingDirection == LEFT ? -1 : 1;

	dashDirection = glm::normalize(dashDirection);

	dashTimer = DASH_TIME;
	canDash = false;
	state = DASHING;
}

void Player::move() {
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		velocity.x = -RUN_MAX_SPEED;
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		velocity.x = RUN_MAX_SPEED;
	else
		velocity.x = 0;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float((int(tileMapDispl.x + posPlayer.x) / PIXEL_SIZE) * PIXEL_SIZE), float((int(tileMapDispl.y + posPlayer.y) / PIXEL_SIZE) * PIXEL_SIZE)));
}

inline void Player::updatePosition(int deltaTime) {
	glm::vec2 deltaVelocity = glm::vec2(velocity.x * deltaTime, velocity.y * deltaTime);

	glm::vec2 deltaVelocityX = glm::vec2(velocity.x * deltaTime, 0.f);

	glm::vec2 deltaVelocityY = glm::vec2(0.f, velocity.y * deltaTime);

	if (velocity.x < 0) {
		facingDirection = LEFT;
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x < 0 || map->collisionMoveLeft(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize)) {
			deltaVelocity.x = 0;
			posPlayer.x = map->getTileSize() * int((posPlayer.x + spriteSize.x) / map->getTileSize()) - hitboxOffset.x;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (velocity.x > 0) {
		facingDirection = RIGHT;
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		int mapSize = map->getMapSize().x*map->getTileSize();
		if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x + hitboxSize.x > mapSize || map->collisionMoveRight(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize))
		{
			deltaVelocity.x = 0;
			posPlayer.x = map->getTileSize() * int(posPlayer.x / map->getTileSize()) + hitboxOffset.x;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else {
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (velocity.y < 0) {
		if (map->collisionMoveUp(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset, hitboxSize)) {
			deltaVelocity.y = 0;
			posPlayer.y = map->getTileSize() * int((posPlayer.y) / map->getTileSize());
		}
	}
	else if (velocity.y > 0) {
		if (map->collisionMoveDown(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset, hitboxSize))
		{
			deltaVelocity.y = 0;
			posPlayer.y = map->getTileSize() * int((posPlayer.y + spriteSize.y - 1) / map->getTileSize());
		}
	}

	setPosition(posPlayer + deltaVelocity);
}
