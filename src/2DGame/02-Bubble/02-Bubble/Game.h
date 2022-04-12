#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"


#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

#define DASH_KEY_UC 88
#define DASH_KEY_LC 120
#define JUMP_KEY_UC 67 
#define JUMP_KEY_LC 99


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
	bool getSpecialKey(int key) const;

	Scene* getScene();

private:
	enum KeyStatus{
		UP,
		PRESSED,
		DOWN
	};
	int currentLevel, score = 0;
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	KeyStatus keyStatus[256];		  // Used to know when they've been pressed.

};


#endif // _GAME_INCLUDE


