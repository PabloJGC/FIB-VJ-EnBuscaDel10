#include <cmath>
#include <glm/glm.hpp>
#include "Sprite.h"

class Cloud 
{
	private:
		Sprite* sprite;
		Texture spritesheet;
		int timer = 0;
		glm::fvec2 pos;
		glm::ivec2 spriteSize, colliderSize;
		int initPos;
		bool moveLeft;

	public:
		Cloud(glm::ivec2 pos, bool moveL);

		void init(ShaderProgram& shaderProgram);
		bool collides(glm::vec2 playerPos, glm::vec2 playerSize);
		void update(int deltaTime, glm::ivec2 mapSize, int tileSize);
		void render();
};

