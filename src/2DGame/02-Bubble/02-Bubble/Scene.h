#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <list>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "PlayerPablo.h"
#include "DustParticle.h"
#include "ExplosionParticle.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level);
	int update(int deltaTime);
	void render();
	void addParticle(Particle* particle);
	void generateDustParticle(glm::ivec2 pos);
	void generateExplosionParticle(glm::ivec2 pos);

private:
	int level;
	void initShaders();
	list<Particle*> particles;

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

