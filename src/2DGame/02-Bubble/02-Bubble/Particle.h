#ifndef _PARTICLE_INCLUDE
#define _PARTICLE_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"

class Particle {
	public:
		Particle(glm::ivec2 pos);
		virtual void init(ShaderProgram& shaderProgram);
		virtual void update(int deltaTime);
		void render() const;
		void free();
		bool exceededMaxLifeTime() const;

	protected:
		Sprite* sprite;
		Texture spritesheet;
		glm::ivec2 spriteSize, initPos, pos;
		int lifeTime;
		int maxLifeTime;
};

#endif _PARTICLE_INCLUDE