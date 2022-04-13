#include "SnowParticle.h"

SnowParticle::SnowParticle(int minY, int maxY, int seed) : Particle(glm::ivec2(0, 0)) {
	srand(seed);
	int y0 = minY/4;
	int y1 = maxY/4;
	int y = y0 + rand()%(y1 - y0);
	pos = glm::ivec2(0, y*4);
	initPos = pos;
	amplitude = float(rand()%32 + 1);
	frequency = 1.f + float(rand()%20)/20.f;
	speed = 0.3f + 0.6*(float(rand()%50)/50.f);
	texture = rand()%2;
}

void SnowParticle::init(ShaderProgram& shaderProgram) {
	Particle::init(shaderProgram);
	maxLifeTime = 5000;
	spriteSize = glm::ivec2(32.f*2.f/8.f, 32.f*2.f/8.f);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(2.f/64.f, 2.f/64.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2((float(texture)*2.f + 40.f)/64.f, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(pos);
}

void SnowParticle::update(int deltaTime) {
	Particle::update(deltaTime);
	pos.y = initPos.y + amplitude*sin(frequency*2.f*3.141593f*float(lifeTime)/1000.f);
	pos.x = initPos.x + float(lifeTime)*speed;
	sprite->setPosition((pos/4)*4);
}
