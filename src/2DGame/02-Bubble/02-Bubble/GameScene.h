#ifndef _GAME_SCENE_INCLUDE
#define _GAME_SCENE_INCLUDE

#include "Scene.h"
#include "TileMap.h"
#include "PlayerPablo.h"
#include "DustParticle.h"
#include "ExplosionParticle.h"
#include "CloudParticle.h"
#include "Quad.h"


// GameScene contains all the entities of our game.
// It is responsible for updating and render them.


class GameScene : public Scene {

public:
	GameScene();
	~GameScene();

	void init(int level);
	int update(int deltaTime);
	void render();
	void generateCloudParticle(int minY, int maxY, int seed);
	void generateDustParticle(glm::ivec2 pos);
	void generateExplosionParticle(glm::ivec2 pos);

private:
	list<CloudParticle*> cloudParticles;
	string levelName;
	Quad* levelTextBg, *resultsTextBg;
	TileMap* map;
	Player* player;
	float cloudTimer;
	bool doShowResults;

};


#endif _GAME_SCENE_INCLUDE

