#pragma once

#include "Renderer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"
#include <vector>

struct Particle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleSystem
{
public:
	ParticleSystem(Shader shader, Texture2D texture, unsigned int amount);
	~ParticleSystem();

	void Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));
	void Draw();
private:
	unsigned int m_Amount;
	unsigned int m_LastUsedParticle;
	std::vector<Particle> particles;

	Shader m_Shader;
	Texture2D m_Texture;

	unsigned int m_VAO;
	void Init();

	unsigned int FirstUnusedParticle();

	void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f));
};