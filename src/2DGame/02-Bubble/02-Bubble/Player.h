#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <cmath>
#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void die();
	void respawn();
	bool update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

protected:
	enum PlayerAnims {
		STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, LOOK_UP_LEFT, LOOK_UP_RIGHT, LOOK_DOWN_LEFT, LOOK_DOWN_RIGHT, STAND_LEFT_NO_DASH, STAND_RIGHT_NO_DASH, MOVE_LEFT_NO_DASH, MOVE_RIGHT_NO_DASH, JUMP_LEFT_NO_DASH, JUMP_RIGHT_NO_DASH, LOOK_UP_LEFT_NO_DASH, LOOK_UP_RIGHT_NO_DASH, LOOK_DOWN_LEFT_NO_DASH, LOOK_DOWN_RIGHT_NO_DASH
	};
	enum State {
		NORMAL,
		DASHING,
		CLIMBING,
		JUMPING,
		WALL_JUMPING
	} state;
	enum FacingDirection {
		RIGHT,
		LEFT
	} facingDirection;
	bool canDash, canClimb, grounded;
	bool dead;
	glm::ivec2 tileMapDispl, spriteSize, hitboxSize, hitboxOffset;
	glm::fvec2 posPlayer, velocity, dashDirection;
	float jumpSpeed;
	int dashTimer;
	int jumpAngle;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	void generateDustParticle();

	void updateState(int deltaTime);
	bool wallAt(FacingDirection facingDirection, int offset) const;
	void wallJump(FacingDirection facingDirection);
	void jump(float speed);
	void dash();
	void move();
	inline void updatePosition(int deltaTime);
	void updateAnimation();
};


#endif // _PLAYER_INCLUDE
