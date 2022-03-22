#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
//#define JUMP_HEIGHT 64
#define RUN_MAX_SPEED 0.25f
#define FALL_SPEED 0.3f
#define JUMP_MAX_SPEED 5.f
#define PIXEL_SIZE 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spriteSize = glm::vec2(32, 32);
	hitboxOffset = glm::vec2(4, 0);
	hitboxSize = glm::vec2(24, 32);

	bJumping = false;
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8./128., 8./256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(4);
	
		// TODO: añadir opción de invertir los keyframes horizontalmente.
		sprite->setAnimationSpeed(STAND_LEFT, 10);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.4375f, 16.f/32.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 10);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.4375f, 16.f/32.f));
		
		// TODO: añadir opción de invertir los keyframes horizontalmente.
		sprite->setAnimationSpeed(MOVE_LEFT, 10);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4375f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(8.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5625f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4375f, 0.53125f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 10);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4375f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5625f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4375f, 0.53125f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

bool Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		velocity.x = -RUN_MAX_SPEED;
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		velocity.x = RUN_MAX_SPEED;
	}
	else
	{
		velocity.x = 0;
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle >= 180)
		{
			bJumping = false;
		}
		else
		{
			velocity.y = -JUMP_MAX_SPEED*cos(3.14159f*jumpAngle/180.f);
			if (jumpAngle > 90) {
				bJumping = !map->collisionMoveDown(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize);
			}
			else if (map->collisionMoveUp(glm::ivec2(posPlayer) + hitboxOffset + glm::ivec2(0, -1), hitboxSize)) {
				bJumping = false;
			}
		}
	}
	else
	{
		velocity.y = FALL_SPEED;
		if (map->collisionMoveDown(glm::ivec2(posPlayer) + hitboxOffset + glm::ivec2(0, 1), hitboxSize))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	updatePosition(deltaTime);
	return posPlayer.y < 0;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float((int(tileMapDispl.x + posPlayer.x)/PIXEL_SIZE)*PIXEL_SIZE), float((int(tileMapDispl.y + posPlayer.y)/ PIXEL_SIZE)*PIXEL_SIZE)));
}

inline void Player::updatePosition(int deltaTime) {
	glm::vec2 deltaVelocity = glm::vec2(velocity.x*deltaTime, velocity.y*deltaTime);
	
	glm::vec2 deltaVelocityX = glm::vec2(velocity.x*deltaTime, 0.f);

	glm::vec2 deltaVelocityY = glm::vec2(0.f, velocity.y*deltaTime);

	if (velocity.x < 0) {
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		if (map->collisionMoveLeft(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize)) {
			deltaVelocity.x = 0;
			posPlayer.x = map->getTileSize() * int((posPlayer.x + spriteSize.x)/map->getTileSize()) - hitboxOffset.x;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (velocity.x > 0) {
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		if (map->collisionMoveRight(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize))
		{
			deltaVelocity.x = 0;
			posPlayer.x = map->getTileSize()*int(posPlayer.x/map->getTileSize()) + hitboxOffset.x;
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
			posPlayer.y = map->getTileSize()*int((posPlayer.y)/map->getTileSize());
		}
	}
	else if (velocity.y > 0) {
		if (map->collisionMoveDown(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset, hitboxSize))
		{
			deltaVelocity.y = 0;
			posPlayer.y = map->getTileSize()*int((posPlayer.y + spriteSize.y - 1)/map->getTileSize());
		}
	}

	setPosition(posPlayer + deltaVelocity);
}


