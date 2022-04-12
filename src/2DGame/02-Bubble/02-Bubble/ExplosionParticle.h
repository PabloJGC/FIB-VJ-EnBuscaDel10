#include "Particle.h"

class ExplosionParticle : public Particle {
	public:
		ExplosionParticle(glm::ivec2 pos, int angleDeg, int texture);

		void init(ShaderProgram& shaderProgram);
		void update(int deltaTime);
	private:
		glm::vec2 dir;
		int texture;
};

