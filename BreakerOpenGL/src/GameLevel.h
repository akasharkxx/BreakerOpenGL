#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"
#include <vector>
#include <fstream>
#include <sstream>

class GameLevel
{
public:
	std::vector<GameObject> m_Bricks;

	GameLevel();

	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

	void Draw(SpriteRenderer& renderer);

	bool IsCompleted();

private:
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth,
		unsigned int levelHeight);
};

// 0 no brick
// 1 solid not destoryable brick
// 2 or more different breakable colored bricks