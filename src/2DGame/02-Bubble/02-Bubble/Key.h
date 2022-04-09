#include <glm/glm.hpp>
#include "Sprite.h"
#include "Chest.h"

class Key {
public:
	Key(glm::ivec2 pos, Chest* chest);

	void init(ShaderProgram& shaderProgram);
	void pickUp(glm::vec2 playerPos, glm::vec2 playerSize);
	void update(int deltaTime);
	void render();
private:
	enum KeyState {
		NORMAL, TAKEN
	} state;
	Sprite* sprite;
	Texture spritesheet;
	glm::fvec2 pos;
	glm::ivec2 spriteSize, colliderSize;

	Chest* chest;
};

