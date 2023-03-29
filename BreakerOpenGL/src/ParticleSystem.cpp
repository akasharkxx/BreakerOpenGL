#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Shader shader, Texture2D texture, unsigned int amount)
    : m_Shader(shader), m_Texture(texture), m_Amount(amount), m_LastUsedParticle(0)
{
    this->Init();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
    for (unsigned int i = 0; i < newParticles; i++)
    {
        int unusedParticles = this->FirstUnusedParticle();
        this->RespawnParticle(this->particles[unusedParticles], object, offset);
    }
    for (unsigned int i = 0; i < this->m_Amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= deltaTime; // reducing life
        if (p.Life > 0.0f)
        {
            p.Position -= p.Velocity * deltaTime;
            p.Color.a -= deltaTime * 2.5f;
        }
    }
}

void ParticleSystem::Draw()
{
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
    this->m_Shader.Use();
    for (const Particle& particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->m_Shader.SetVector2f("offset", particle.Position);
            this->m_Shader.SetVector4f("color", particle.Color);
            this->m_Texture.Bind();
            GLCall(glBindVertexArray(this->m_VAO));
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
            GLCall(glBindVertexArray(0));
        }
    }

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void ParticleSystem::Init()
{
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    GLCall(glGenVertexArrays(1, &this->m_VAO));
    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindVertexArray(this->m_VAO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GLCall(glBindVertexArray(0));

    for (unsigned int i = 0; i < this->m_Amount; ++i)
    {
        this->particles.emplace_back(Particle());
    }
}

unsigned int ParticleSystem::FirstUnusedParticle()
{
    for (unsigned int i = m_LastUsedParticle; i < this->m_Amount; i++)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < m_LastUsedParticle; i++)
    {
        if (this->particles[i].Life <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    m_LastUsedParticle = 0;
    return 0;
}

void ParticleSystem::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);

    particle.Position = object.m_Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.m_Velocity * 0.1f;
}
