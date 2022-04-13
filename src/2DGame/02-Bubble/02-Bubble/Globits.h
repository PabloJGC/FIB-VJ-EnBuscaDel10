#ifndef _GLOBITS_INCLUDE
#define _GLOBITS_INCLUDE

#include <cmath>
#include <glm/glm.hpp>
#include "Sprite.h"

class Globits {
	public:
		Globits(glm::ivec2 pos);

		void init(ShaderProgram& shaderProgram);
		bool collides(glm::vec2 playerPos, glm::vec2 playerSize);
		void update(int deltaTime);
		void render();
	private:
		enum GlobitsState {
			NORMAL, BLOWN_UP
		} state;
		Sprite* sprite;
		Texture spritesheet;
		int timer = 0;
		glm::fvec2 pos;
		glm::ivec2 spriteSize, colliderSize;
		int initPos;
};

#endif _GLOBITS_INCLUDE
