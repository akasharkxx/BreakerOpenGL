#include "BallObject.h"

BallObject::BallObject() : 
    GameObject(), Radius(0), Stuck(false)
{
}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity),
    Radius(radius), Stuck(true)
{
}

glm::vec2 BallObject::Move(float deltaTime, unsigned int window_width)
{
    if (!this->Stuck)
    {
        this->m_Position += this->m_Velocity * deltaTime;

        if (this->m_Position.x <= 0.0f)
        {
            this->m_Velocity.x = -this->m_Velocity.x;
            this->m_Position.x = 0.0f;
        }
        else if (this->m_Position.x + this->m_Size.x >= window_width)
        {
            this->m_Velocity.x = -this->m_Velocity.x;
            this->m_Position.x = window_width - this->m_Size.x;
        }
        else if (this->m_Position.y <= 0.0f)
        {
            this->m_Velocity.y = -this->m_Velocity.y;
            this->m_Position.y = 0.0f;
        }
    }

    return this->m_Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->m_Position = position;
    this->m_Velocity = velocity;
    this->Stuck = true;
}
