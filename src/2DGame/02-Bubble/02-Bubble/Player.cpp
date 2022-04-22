#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 0.3125f
#define RUN_MAX_SPEED 0.25f
#define FALL_SPEED 0.25f
#define CLIMB_FALL_SPEED 0.05f
#define JUMP_MAX_SPEED 0.5f
#define SPRING_MAX_SPEED 0.9f
#define DASH_TIME 150
#define DASH_MAX_SPEED 0.7f
#define DASH_MIN_SPEED 0.4f
#define WALL_JUMP_TIME 200
#define WALL_JUMP_MAX_SPEED 0.7f
#define WALL_JUMP_MIN_SPEED 0.3f
#define WALL_JUMP_DISTANCE 8
#define PIXEL_SIZE 4

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	state = NORMAL;

	spriteSize = glm::vec2(32, 32);
	hitboxOffset = glm::vec2(4, 0);
	hitboxSize = glm::vec2(24, 32);

	canDash = true;
	canClimb = false;
	aboveCloud = false;
	dead = false;

	dashDirection = glm::ivec2(0);
	spritesheet.loadFromFile("images/textures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8. / 128., 8. / 256), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(20);

	sprite->setAnimationSpeed(STAND_LEFT, 10);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(7.f / 16.f, 19.f / 32.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 10);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(7.f / 16.f, 16.f / 32.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(7.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(8.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(9.f / 16.f, 19.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(7.f / 16.f, 20.f / 32.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9.f / 16.f, 16.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7.f / 16.f, 17.f / 32.f));

	sprite->setAnimationSpeed(JUMP_LEFT, 10);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(7.f / 16.f, 20.f / 32.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 10);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(7.f / 16.f, 17.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_LEFT, 10);
	sprite->addKeyframe(LOOK_UP_LEFT, glm::vec2(8.f / 16.f, 21.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_RIGHT, 10);
	sprite->addKeyframe(LOOK_UP_RIGHT, glm::vec2(8.f / 16.f, 18.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_LEFT, 10);
	sprite->addKeyframe(LOOK_DOWN_LEFT, glm::vec2(7.f / 16.f, 21.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_RIGHT, 10);
	sprite->addKeyframe(LOOK_DOWN_RIGHT, glm::vec2(7.f / 16.f, 18.f / 32.f));


	sprite->setAnimationSpeed(STAND_LEFT_NO_DASH, 10);
	sprite->addKeyframe(STAND_LEFT_NO_DASH, glm::vec2(7.f / 16.f, 25.f / 32.f));

	sprite->setAnimationSpeed(STAND_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(STAND_RIGHT_NO_DASH, glm::vec2(7.f / 16.f, 22.f / 32.f));

	sprite->setAnimationSpeed(MOVE_LEFT_NO_DASH, 10);
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(7.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(8.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(9.f / 16.f, 25.f / 32.f));
	sprite->addKeyframe(MOVE_LEFT_NO_DASH, glm::vec2(7.f / 16.f, 26.f / 32.f));

	sprite->setAnimationSpeed(MOVE_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(7.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(8.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(9.f / 16.f, 22.f / 32.f));
	sprite->addKeyframe(MOVE_RIGHT_NO_DASH, glm::vec2(7.f / 16.f, 23.f / 32.f));

	sprite->setAnimationSpeed(JUMP_LEFT_NO_DASH, 10);
	sprite->addKeyframe(JUMP_LEFT_NO_DASH, glm::vec2(7.f / 16.f, 26.f / 32.f));

	sprite->setAnimationSpeed(JUMP_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(JUMP_RIGHT_NO_DASH, glm::vec2(7.f / 16.f, 23.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_LEFT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_UP_LEFT_NO_DASH, glm::vec2(8.f / 16.f, 27.f / 32.f));

	sprite->setAnimationSpeed(LOOK_UP_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_UP_RIGHT_NO_DASH, glm::vec2(8.f / 16.f, 24.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_LEFT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_DOWN_LEFT_NO_DASH, glm::vec2(7.f / 16.f, 27.f / 32.f));

	sprite->setAnimationSpeed(LOOK_DOWN_RIGHT_NO_DASH, 10);
	sprite->addKeyframe(LOOK_DOWN_RIGHT_NO_DASH, glm::vec2(7.f / 16.f, 24.f / 32.f));

	sprite->changeAnimation(STAND_RIGHT);
	facingDirection = RIGHT;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

bool Player::update(int deltaTime)
{
	if (dead) {
		respawn();
	}
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
		velocity.y = min(FALL_SPEED, -jumpSpeed * cos(3.14159f * jumpAngle / 180.f));
		break;
	}
	case DASHING: {
		velocity = dashDirection * (DASH_MIN_SPEED + (DASH_MAX_SPEED - DASH_MIN_SPEED) * (float(dashTimer) / DASH_TIME));
		break;
	}
	case CLIMBING: {
		move();
		velocity.y = CLIMB_FALL_SPEED;
		break;
	}
	case WALL_JUMPING: {
		velocity = dashDirection * (WALL_JUMP_MIN_SPEED + (WALL_JUMP_MAX_SPEED - WALL_JUMP_MIN_SPEED) * (float(dashTimer) / WALL_JUMP_TIME));
		break;
	}
	}

	updatePosition(deltaTime);

	if (map->enteredDeathZone(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize, Game::instance().getGodMode()))
		die();
	else {
		map->breakFragileTiles(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize);
		map->pickUpKeys(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize);
		if (state == DASHING) {
			int wall;
			bool bt = map->breakBreakableTiles(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize, wall);
			if (bt) {
				canDash = true;
				if (wall != -1) {
					wallJump(FacingDirection(wall));
				}
			}

		}
		if (map->enteredSpring(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize)) {
			canDash = true;
			jump(SPRING_MAX_SPEED);
		}
		if (map->enteredGlobits(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize)) {
			Game::instance().playSound("audio/powerup.wav");
			canDash = true;
		}
		if (map->enteredFile(glm::ivec2(posPlayer) + hitboxOffset, hitboxSize)) {
			Game::instance().playSound("audio/score.wav");
			Game::instance().increaseScore();
		}
	}

	updateAnimation();

	return posPlayer.y < 0;
}

void Player::die() {
	Game::instance().playSound("audio/death.mp3");
	Game::instance().increaseDeathCount();
	((GameScene*)(Game::instance().getScene()))->generateExplosionParticle(posPlayer);
	dead = true;
}

void Player::respawn() {
	map->reset();
	posPlayer = map->getPlayerInitPos();
	canDash = true;
	canClimb = false;
	dead = false;
	state = NORMAL;
	facingDirection = RIGHT;
	sprite->changeAnimation(STAND_RIGHT);
}

void Player::updateState(int deltaTime) {
	int dummy;
	bool grounded_temp = grounded;
	grounded = map->collisionMoveDown(glm::ivec2(posPlayer) + hitboxOffset + glm::ivec2(0, 1), hitboxSize, dummy);
	if (!grounded_temp && grounded)
		generateDustParticle();
	
	if (grounded && state != DASHING)
		canDash = true;
	canClimb = !grounded && !aboveCloud && velocity.y > 0;

	switch (state) {
	case NORMAL: {
		if (Game::instance().getJumpKeyPressed()) {
			if (grounded || aboveCloud) {
				jump(JUMP_MAX_SPEED);
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
			else if (canClimb && ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && map->collisionMoveRight(glm::ivec2(posPlayer + glm::vec2(1.f, 0.f)) + hitboxOffset, hitboxSize, dummy)) ||
				(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && map->collisionMoveLeft(glm::ivec2(posPlayer + glm::vec2(-1.f, 0.f)) + hitboxOffset, hitboxSize, dummy)))) {
				state = CLIMBING;
			}
		}
		break;
	}
	case JUMPING: {
		jumpAngle += JUMP_ANGLE_STEP*deltaTime;
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
				(jumpAngle > 90 && (grounded || aboveCloud)))
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
			else if (grounded || aboveCloud || !((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && wallAt(RIGHT, 1)) ||
				(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && wallAt(LEFT, 1)))) {
				state = NORMAL;
			}
		}
		break;
	}
	case WALL_JUMPING: {
		if (Game::instance().getJumpKeyPressed()) {
			if (wallAt(RIGHT, WALL_JUMP_DISTANCE)) {
				wallJump(RIGHT);
			}
			else if (wallAt(LEFT, WALL_JUMP_DISTANCE)) {
				wallJump(LEFT);
			}
		}
		if (canDash && Game::instance().getDashKeyPressed()) {
			dash();
		}
		else {
			dashTimer -= deltaTime;
			if (dashTimer <= 0) {
				state = NORMAL;
			}
		}
		break;
	}
	}
}

void Player::generateDustParticle() {
	((GameScene*)(Game::instance().getScene()))->generateDustParticle(posPlayer);;
}

bool Player::wallAt(FacingDirection direction, int offset) const {
	int dummy;
	if (direction == RIGHT)
		return map->collisionMoveRight(glm::ivec2(posPlayer + glm::vec2(offset, 0)) + hitboxOffset, hitboxSize, dummy);
	return map->collisionMoveLeft(glm::ivec2(posPlayer + glm::vec2(-offset, 0)) + hitboxOffset, hitboxSize, dummy);
}

void Player::wallJump(FacingDirection facingDirection) {
	Game::instance().playSound("audio/jump.wav");
	generateDustParticle();
	dashDirection.y = -1;
	dashDirection.x = facingDirection == LEFT ? 1 : -1;

	dashDirection = glm::normalize(dashDirection);

	dashTimer = WALL_JUMP_TIME;
	state = WALL_JUMPING;
}

void Player::jump(float speed) {
	Game::instance().playSound("audio/jump.wav");
	generateDustParticle();
	state = JUMPING;
	jumpAngle = 0;
	jumpSpeed = speed;
}

void Player::dash() {
	Game::instance().playSound("audio/dash.wav");
	generateDustParticle();
	map->startleWingedFiles();
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
	canDash = false || Game::instance().getInfiniteDashMode();
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
		/*if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);*/
		int colX = -1;
		if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x < 0 || map->collisionMoveLeft(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize, colX)) {
			deltaVelocity.x = 0;
			velocity.x = 0;
			posPlayer.x = map->getTileSize()*(colX + 1) - hitboxOffset.x;
			//sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (velocity.x > 0) {
		facingDirection = RIGHT;
		/*if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);*/
		int mapSize = map->getMapSize().x*map->getTileSize();
		int colX = map->getMapSize().x;
		if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x + hitboxSize.x > mapSize || map->collisionMoveRight(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize, colX)) {
			deltaVelocity.x = 0;
			velocity.x = 0;
			posPlayer.x = map->getTileSize()*(colX - 1) + hitboxOffset.x;
			//sprite->changeAnimation(STAND_RIGHT);
		}
	}
	/*else {
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}*/

	if (velocity.y < 0) {
		int colY;
		if (map->collisionMoveUp(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset, hitboxSize, colY)) {
			deltaVelocity.y = 0;
			velocity.y = 0;
			posPlayer.y = map->getTileSize()*(colY + 1) - hitboxOffset.y;;
		}
	}
	else if (velocity.y > 0) {
		int colY;
		if (map->collisionMoveDown(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset, hitboxSize, colY))
		{
			deltaVelocity.y = 0;
			velocity.y = 0;
			posPlayer.y = map->getTileSize()*(colY - 1) + hitboxOffset.y;
		}
		float cloudSpeed;
		int colTile;
		if (map->isAboveCloud(glm::ivec2(posPlayer + deltaVelocity) + hitboxOffset + glm::ivec2(0, hitboxSize.y - 4), glm::ivec2(hitboxSize.x, 4), cloudSpeed, colTile)) {
			aboveCloud = true;
			canDash = true;
			deltaVelocityX.x += cloudSpeed * deltaTime;
			deltaVelocity.x += cloudSpeed * deltaTime;
			deltaVelocity.y = 0;
			int mapSize = map->getMapSize().x * map->getTileSize();
			int colX = map->getMapSize().x;
			if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x + hitboxSize.x > mapSize || map->collisionMoveRight(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize, colX)) {
				deltaVelocity.x = 0;
				posPlayer.x = map->getTileSize() * (colX - 1) + hitboxOffset.x;
			}
			colX = -1;
			if (glm::ivec2(posPlayer + deltaVelocityX).x + hitboxOffset.x < 0 || map->collisionMoveLeft(glm::ivec2(posPlayer + deltaVelocityX) + hitboxOffset, hitboxSize, colX)) {
				posPlayer.x = map->getTileSize() * (colX + 1) - hitboxOffset.x;
				deltaVelocity.x = 0;
			}
			posPlayer.y = map->getTileSize() * (colTile - 1) + hitboxOffset.y;
		}
		else
			aboveCloud = false;
	}

	setPosition(posPlayer + deltaVelocity);
}

void Player::updateAnimation() {
	if (canDash) {
		if (grounded || aboveCloud) {
			if (velocity.x < 0 && sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			else if (velocity.x > 0 && sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			else if (velocity.x == 0) {
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
					sprite->changeAnimation(facingDirection == LEFT ? LOOK_UP_LEFT : LOOK_UP_RIGHT);
				else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
					sprite->changeAnimation(facingDirection == LEFT ? LOOK_DOWN_LEFT : LOOK_DOWN_RIGHT);
				else
					sprite->changeAnimation(facingDirection == LEFT ? STAND_LEFT : STAND_RIGHT);
			}
		}
		else
			sprite->changeAnimation(facingDirection == LEFT ? JUMP_LEFT : JUMP_RIGHT);
	}
	else {
		if (grounded || aboveCloud) {
			if (velocity.x < 0 && sprite->animation() != MOVE_LEFT_NO_DASH)
				sprite->changeAnimation(MOVE_LEFT_NO_DASH);
			else if (velocity.x > 0 && sprite->animation() != MOVE_RIGHT_NO_DASH)
				sprite->changeAnimation(MOVE_RIGHT_NO_DASH);
			else if (velocity.x == 0) {
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
					sprite->changeAnimation(facingDirection == LEFT ? LOOK_UP_LEFT_NO_DASH : LOOK_UP_RIGHT_NO_DASH);
				else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
					sprite->changeAnimation(facingDirection == LEFT ? LOOK_DOWN_LEFT_NO_DASH : LOOK_DOWN_RIGHT_NO_DASH);
				else
					sprite->changeAnimation(facingDirection == LEFT ? STAND_LEFT_NO_DASH : STAND_RIGHT_NO_DASH);
			}
		}
		else
			sprite->changeAnimation(facingDirection == LEFT ? JUMP_LEFT_NO_DASH : JUMP_RIGHT_NO_DASH);
	}
}
