#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 240
#define FALL_STEP 6
#define RUN_MAX_SPEED 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	size = glm::vec2(32, 32);
	bJumping = false;
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, glm::vec2(8./128., 8./256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(4);
	
		// TODO: añadir opción de invertir los keyframes horizontalmente.
		sprite->setAnimationSpeed(STAND_LEFT, 10);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(7.f/16.f, 16.f/32.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 10);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(7.f/16.f, 16.f/32.f));
		
		// TODO: añadir opción de invertir los keyframes horizontalmente.
		sprite->setAnimationSpeed(MOVE_LEFT, 10);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(7.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(8.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(9.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(7.f/16.f, 17.f/32.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 10);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9.f/16.f, 16.f/32.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7.f/16.f, 17.f/32.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= RUN_MAX_SPEED;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += RUN_MAX_SPEED;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += RUN_MAX_SPEED;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= RUN_MAX_SPEED;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			if (map->collisionMoveUp(posPlayer, size, &posPlayer.y))
				bJumping = false;
			else {
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, size, &posPlayer.y);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, size, &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




