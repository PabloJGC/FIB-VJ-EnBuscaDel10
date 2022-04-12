#include "ExplosionParticle.h"

#define SPEED 0.15f

ExplosionParticle::ExplosionParticle(glm::ivec2 pos, int angleDeg, int texture) : Particle(pos) {
	float angle = (float(angleDeg)/360.f)*2*3.14159f;
	dir = glm::vec2(sin(angle), cos(angle));
	this->texture = texture;
}

void ExplosionParticle::init(ShaderProgram& shaderProgram) {
	Particle::init(shaderProgram);
	maxLifeTime = 150;
	spriteSize = glm::ivec2(32.f*3.f/8.f, 32.f*3.f/8.f);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(3.f / 64.f, 3.f / 64.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2((float(texture)*3.f + 24.f)/64.f, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(pos);
}

void ExplosionParticle::update(int deltaTime) {
	Particle::update(deltaTime);
	pos = initPos + glm::ivec2(dir*float(lifeTime)*SPEED);
	sprite->setPosition((pos/4)*4);
}