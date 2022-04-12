#include "Particle.h"

class DustParticle : public Particle {
	public:
		DustParticle(glm::ivec2 pos) : Particle(pos) {}

		void init(ShaderProgram& shaderProgram);
		void update(int deltaTime);
};
