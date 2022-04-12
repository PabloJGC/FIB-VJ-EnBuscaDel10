#pragma once
#include "Player.h"
class PlayerPablo : public Player {
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
};

