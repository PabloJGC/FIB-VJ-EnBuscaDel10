#include <cmath>
#include <glm/glm.hpp>
#include "Sprite.h"

class File
{
	private:
		enum FileState {
			NORMAL, TAKEN
		} state;
		Sprite* sprite;
		Texture spritesheet;
		int timer = 0;
		glm::fvec2 pos;
		glm::ivec2 spriteSize, colliderSize;
		int initPos;

	public:
		File(glm::ivec2 pos);

		void init(ShaderProgram& shaderProgram);
		bool collides(glm::vec2 playerPos, glm::vec2 playerSize);
		void update(int deltaTime);
		void render();
};

