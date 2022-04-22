#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <list>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "SnowParticle.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	virtual void init(int level);
	virtual int update(int deltaTime);
	virtual void render() {};
	void setCameraOffset(int x, int y);
	void generateSnowParticle(int minY, int maxY, int time);
	void addParticle(Particle* particle);

protected:
	int level;
	void initShaders();
	list<Particle*> particles;

protected:
	Text text;
	ShaderProgram texProgram, simpleProgram;
	float currentTime, snowTimer;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

