#include "DustParticle.h"

#define KEYFRAME_TIME 100
#define SPEED 0.05f

void DustParticle::init(ShaderProgram& shaderProgram) {
	Particle::init(shaderProgram);
	maxLifeTime = KEYFRAME_TIME*3;
	spriteSize = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(8.f/64.f, 8.f/64.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 1000/KEYFRAME_TIME);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(1.f/8.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(2.f/8.f, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(pos);
}

void DustParticle::update(int deltaTime) {
	Particle::update(deltaTime);
	pos.x = initPos.x + int(lifeTime*SPEED);
	sprite->setPosition((pos/4)*4);
}