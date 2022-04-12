#include "Particle.h"

Particle::Particle(glm::ivec2 pos) {
	this->pos = pos;
	initPos = pos;
	lifeTime = 0;
}

void Particle::init(ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/particles.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Particle::update(int deltaTime) {
	lifeTime += deltaTime;
	sprite->update(deltaTime);
}

void Particle::render() const {
	sprite->render();
}

void Particle::free() {
	sprite->free();
	delete sprite;
}

bool Particle::exceededMaxLifeTime() const {
	return lifeTime > maxLifeTime;
}