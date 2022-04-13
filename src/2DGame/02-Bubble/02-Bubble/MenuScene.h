#ifndef _MENU_SCENE_INCLUDE
#define _MENU_SCENE_INCLUDE

#include "Scene.h"
#include "TexturedQuad.h"

class MenuScene : public Scene {
	public:
		MenuScene();
		~MenuScene();

		void init(int level);
		int update(int deltaTime);
		void render();
	private:
		TexturedQuad* texQuad;
		Texture textures;
};

#endif _MENU_SCENE_INCLUDE