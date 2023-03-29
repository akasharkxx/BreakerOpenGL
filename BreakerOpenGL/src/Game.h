#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleSystem.h"
#include <vector>

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game
{
public:
	GameState m_State;
	bool m_Keys[1024];
	unsigned int m_Width, m_Height;
	std::vector<GameLevel> m_Levels;
	unsigned int m_Level;

	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	void DoCollisions();
	bool CheckForCollision(GameObject& one, GameObject& two);
	Collision CheckForCollision(BallObject& one, GameObject& two);

	Direction VectorDirection(glm::vec2 target);
	
	void ResetLevel();
	void ResetPlayer();

private:
	SpriteRenderer* Renderer;

	GameObject* m_Player;
	BallObject* m_Ball;
	ParticleSystem* m_Particle;
};
