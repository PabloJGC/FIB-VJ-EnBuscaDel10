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
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void die();
	void respawn();
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
	bool canDash, canClimb, grounded, aboveCloud;
	bool dead;
	glm::ivec2 tileMapDispl, spriteSize, hitboxSize, hitboxOffset;
	glm::fvec2 posPlayer, velocity, dashDirection;
	float jumpSpeed;
	int dashTimer;
	int jumpAngle;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	void updateState(int deltaTime);
	bool wallAt(FacingDirection facingDirection, int offset) const;
	void wallJump(FacingDirection facingDirection);
	void jump(float speed);
	void dash();
	void move();
	inline void updatePosition(int deltaTime);
};


#endif // _PLAYER_INCLUDE
