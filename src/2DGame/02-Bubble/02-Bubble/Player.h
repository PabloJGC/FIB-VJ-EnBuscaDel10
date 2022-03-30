#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	bool update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
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
	int dashTimer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	void updateState(int deltaTime);
	bool wallAt(FacingDirection facingDirection, int offset) const;
	void wallJump(FacingDirection facingDirection);
	void dash();
	void move();
	inline void updatePosition(int deltaTime);
};


#endif // _PLAYER_INCLUDE
