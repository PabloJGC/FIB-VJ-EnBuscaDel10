#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "GameScene.h"
#include "MenuScene.h"

#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

#define DASH_KEY_UC 88
#define DASH_KEY_LC 120
#define JUMP_KEY_UC 67 
#define JUMP_KEY_LC 99
#define GOD_MODE_KEY_UC 71
#define GOD_MODE_KEY_LC 103
#define SLOW_MODE_KEY_UC 83
#define SLOW_MODE_KEY_LC 115
#define INFINITE_DASH_MODE_KEY_UC 68
#define INFINITE_DASH_MODE_KEY_LC 100


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void setBackgroundColor(float red, float green, float blue);
	void increaseScore();
	void increaseDeathCount();
	void playSound(char* file);

	int getScore() { return score; }
	int getDeathCount() { return deathCount; }
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getJumpKeyPressed();
	bool getDashKeyPressed();
	bool getGodModeKeyPressed();
	bool getSlowModeKeyPressed();
	bool getInfiniteDashModeKeyPressed();
	bool getSpecialKey(int key) const;

	bool getGodMode();
	bool getSlowMode();
	bool getInfiniteDashMode();

	Scene* getScene();

private:
	ISoundEngine* engine;
	enum KeyStatus{
		UP,
		PRESSED,
		DOWN
	};
	bool godMode, slowMode, infiniteDashMode;
	int currentLevel, score = 0, deathCount = 0;
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	KeyStatus keyStatus[256];		  // Used to know when they've been pressed.

	void updateMode();

};


#endif // _GAME_INCLUDE


