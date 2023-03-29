#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
	: m_State(GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height),
		Renderer(), m_Player(), m_Ball(), m_Level(0)
{
}

Game::~Game()
{
	delete Renderer;
	delete m_Player;
	delete m_Ball;
	delete m_Particle;
}

void Game::Init()
{
	// Loading shader
	ResourceManager::LoadShader("res/shaders/SpriteShader.vshader", "res/shaders/SpriteShader.fshader", "sprite");
	ResourceManager::LoadShader("res/shaders/Particle.vshader", "res/shaders/Particle.fshader", "particle");
	// configure shader
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->m_Width),
		static_cast<float>(this->m_Height), 0.0f, -1.0f, 1.0f);

	Shader spriteShader = ResourceManager::GetShader("sprite");
	spriteShader.Use().SetInteger("image", 0);
	spriteShader.SetMatrix4("projection", projection);

	Shader particleShader = ResourceManager::GetShader("particle");
	particleShader.Use().SetInteger("sprite", 0);
	particleShader.SetMatrix4("projection", projection);

	ResourceManager::LoadTexture("res/textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("res/textures/background_1.png", false, "background");
	ResourceManager::LoadTexture("res/textures/Block_3.png", false, "block");
	ResourceManager::LoadTexture("res/textures/Block_solid_1.png", false, "block_solid");
	ResourceManager::LoadTexture("res/textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("res/textures/particle.png", true, "particle");

	this->Renderer = new SpriteRenderer(spriteShader);
	this->m_Particle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);

	GameLevel one; one.Load("res/levels/one.lvl", this->m_Width, this->m_Height / 2);
	GameLevel two; two.Load("res/levels/two.lvl", this->m_Width, this->m_Height / 2);
	GameLevel three; three.Load("res/levels/three.lvl", this->m_Width, this->m_Height / 2);
	GameLevel four; four.Load("res/levels/four.lvl", this->m_Width, this->m_Height / 2);

	this->m_Levels.push_back(one);
	this->m_Levels.push_back(two);
	this->m_Levels.push_back(three);
	this->m_Levels.push_back(four);
	this->m_Level = 2;

	glm::vec2 playerPos = glm::vec2(this->m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, 
								this->m_Height - PLAYER_SIZE.y);

	m_Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
											-BALL_RADIUS * 2.0f);

	m_Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
								ResourceManager::GetTexture("face"));
}

void Game::ProcessInput(float deltaTime)
{
	if (this->m_State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * deltaTime;

		if (this->m_Keys[GLFW_KEY_A])
		{
			if (m_Player->m_Position.x >= 0.0f)
			{
				m_Player->m_Position.x -= velocity;
				if (m_Ball->Stuck)
				{
					m_Ball->m_Position.x -= velocity;	
				}
			}
		}

		if (this->m_Keys[GLFW_KEY_D])
		{
			if (m_Player->m_Position.x <= this->m_Width - m_Player->m_Size.x)
			{
				m_Player->m_Position.x += velocity;
				if (m_Ball->Stuck)
				{
					m_Ball->m_Position.x += velocity;
				}
			}
		}

		if (this->m_Keys[GLFW_KEY_SPACE])
		{
			m_Ball->Stuck = false;
		}
	}
}

void Game::Update(float deltaTime)
{
	m_Ball->Move(deltaTime, this->m_Width);

	this->DoCollisions();

	m_Particle->Update(deltaTime, *m_Ball, 2, glm::vec2(m_Ball->m_Size / 2.0f));

	if (m_Ball->m_Position.y >= this->m_Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}

void Game::Render()
{
	/*auto texture = ResourceManager::GetTexture("face");
	this->Renderer->DrawSprite(texture,
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f,
		glm::vec3(0.0f, 1.0f, 0.0f));*/

	if (this->m_State == GAME_ACTIVE)
	{
		auto texture = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(texture, glm::vec2(0.0f, 0.0f),
			glm::vec2(this->m_Width, this->m_Height), 0.0f);

		this->m_Levels[this->m_Level].Draw(*Renderer);

		m_Player->Draw(*Renderer);
		m_Particle->Draw();
		m_Ball->Draw(*Renderer);
	}
}
// Can be replaced with separating axis theorem or Box2D library or space partitioning
void Game::DoCollisions()
{
	for (GameObject& box : this->m_Levels[this->m_Level].m_Bricks)
	{
		if (!box.m_Destroyed)
		{
			Collision collision = CheckForCollision(*m_Ball, box);
			if (std::get<0>(collision))
			{
				if (!box.m_IsSolid)
					box.m_Destroyed = true;

				// collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				
				if (dir == LEFT || dir == RIGHT)
				{
					m_Ball->m_Velocity.x = -m_Ball->m_Velocity.x;
					float penetration = m_Ball->Radius - std::abs(diff_vector.x);

					if (dir == LEFT)
					{
						m_Ball->m_Position.x += penetration;
					}
					else 
					{
						m_Ball->m_Position.x -= penetration;
					}
				}
				else
				{
					m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y;
					float penetration = m_Ball->Radius - std::abs(diff_vector.y);

					if (dir == UP)
					{
						m_Ball->m_Position.y -= penetration;
					}
					else
					{
						m_Ball->m_Position.y -= penetration;
					}
				}
			}
		}
	}

	Collision collisionBetweenPaddleAndPlayer = CheckForCollision(*m_Ball, *m_Player);
	if (!m_Ball->Stuck && std::get<0>(collisionBetweenPaddleAndPlayer))
	{
		float centerBoard = m_Player->m_Position.x + m_Player->m_Size.x / 2.0f;
		float distance = (m_Ball->m_Position.x + m_Ball->Radius) - centerBoard;
		float percentage = distance / (m_Player->m_Size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = m_Ball->m_Velocity;
		m_Ball->m_Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		m_Ball->m_Velocity.y = -1.0f * glm::abs(m_Ball->m_Velocity.y);
		m_Ball->m_Velocity = glm::normalize(m_Ball->m_Velocity) * glm::length(oldVelocity);
	}
}

bool Game::CheckForCollision(GameObject& one, GameObject&two)
{
	bool collisionX = one.m_Position.x + one.m_Size.x > two.m_Position.x &&
		two.m_Position.x + two.m_Size.x > one.m_Position.x;

	bool collisionY = one.m_Position.y + one.m_Size.y > two.m_Size.y &&
		two.m_Position.y + two.m_Size.y > one.m_Position.y;

	return collisionX && collisionY;
}

Collision Game::CheckForCollision(BallObject& one, GameObject& two)
{
	glm::vec2 center(one.m_Position + one.Radius);
	// aabb info
	glm::vec2 aabb_half_extents(two.m_Size.x / 2.0f, two.m_Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.m_Position.x + aabb_half_extents.x, 
		two.m_Position.y + aabb_half_extents.y
	);
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);// Clamping to be between the rectangle

	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	if (glm::length(difference) < one.Radius)
	{
		return std::make_tuple(true, VectorDirection(difference), difference);
	}
	else 
	{
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
	}
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] =
	{
		glm::vec2(0.0f, 1.0f), // up
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
}

void Game::ResetLevel()
{
	if (this->m_Level == 0)
		this->m_Levels[0].Load("res/levels/one.lvl", this->m_Width, this->m_Height / 2);
	else if (this->m_Level == 1)
		this->m_Levels[1].Load("res/levels/two.lvl", this->m_Width, this->m_Height / 2);
	else if (this->m_Level == 2)
		this->m_Levels[2].Load("res/levels/three.lvl", this->m_Width, this->m_Height / 2);
	else if (this->m_Level == 3)
		this->m_Levels[3].Load("res/levels/four.lvl", this->m_Width, this->m_Height / 2);
}

void Game::ResetPlayer()
{
	m_Player->m_Size = PLAYER_SIZE;
	m_Player->m_Position = glm::vec2(this->m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->m_Height - PLAYER_SIZE.y);
	m_Ball->Reset(m_Player->m_Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

	LOG(m_Ball->m_Position.x);
}
