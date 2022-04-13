#include "Particle.h"
#include <stdlib.h>     /* srand, rand */

class CloudParticle : public Particle {
public:
	CloudParticle(int minY, int maxY, int seed);

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
private:
	int texture;
	float amplitude, frequency, speed;
};