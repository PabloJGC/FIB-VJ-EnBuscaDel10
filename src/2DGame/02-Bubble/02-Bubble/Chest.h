#include <glm/glm.hpp>
#include "Sprite.h"

class Chest {
	public:
		Chest(glm::ivec2 pos);

		void init(ShaderProgram& shaderProgram);
		bool collides(glm::vec2 playerPos, glm::vec2 playerSize);
		void unlock();
		void update(int deltaTime);
		void render();
	private:
		enum ChestState {
			LOCKED, NORMAL, TAKEN
		} state;
		Sprite* sprite;
		Texture spritesheet;
		int timer = 0;
		glm::fvec2 pos;
		float initPos;
		glm::ivec2 spriteSize, colliderSize;
};

