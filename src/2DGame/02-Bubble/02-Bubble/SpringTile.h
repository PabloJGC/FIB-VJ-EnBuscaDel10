#include "Tile.h"

class SpringTile : public Tile {
	public:
		SpringTile(int id, glm::ivec2 pos, int size);

		void update(int deltaTime);
		void render() const;
		void init(ShaderProgram& shaderProgram);

		void setPressed();

		bool isSpring() const { return true; }
		bool isDynamic() const { return true; }
	private:
		enum SpringState {
			NORMAL, PRESSED
		} state;

		int timer;
		Sprite* sprite;
		Texture spritesheet;
		glm::ivec2 spriteSize;
};

