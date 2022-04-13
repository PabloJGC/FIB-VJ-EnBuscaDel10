#include "CloudParticle.h"

CloudParticle::CloudParticle(int minY, int maxY, int seed) : Particle(glm::ivec2(0, 0)) {
	srand(seed);
	int y0 = minY/4;
	int y1 = maxY/4;
	int y = y0 + rand()%(y1 - y0);
	spriteSize.x = 6.f*32.f + 32.f*(rand()%4);
	spriteSize.y = 48.f + 4.f*(rand()%2); 
	//glm::ivec2(6.f * 32.f, 32.f * 2.f);
	pos = glm::ivec2(-spriteSize.x, y*4);
	initPos = pos;
	speed = 0.1f + 0.4*(float(rand()%50)/50.f);
}

void CloudParticle::init(ShaderProgram& shaderProgram) {
	Particle::init(shaderProgram);
	maxLifeTime = 10000;
	sprite = Sprite::createSprite(spriteSize, glm::vec2(1.f/64.f, 2.f/64.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(48.f/64.f, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(pos);
}

void CloudParticle::update(int deltaTime) {
	Particle::update(deltaTime);
	pos.x = initPos.x + float(lifeTime)*speed;
	sprite->setPosition((pos/4)*4);
}
