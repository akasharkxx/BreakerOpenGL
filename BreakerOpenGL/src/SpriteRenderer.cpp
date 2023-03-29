#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    this->m_Shader = shader;
    this->InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    GLCall(glDeleteVertexArrays(1, &this->m_QuadVAO));
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    this->m_Shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->m_Shader.SetMatrix4("model", model);
    this->m_Shader.SetVector3f("spriteColor", color);

    GLCall(glActiveTexture(GL_TEXTURE0));
    texture.Bind();

    GLCall(glBindVertexArray(this->m_QuadVAO));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
    GLCall(glBindVertexArray(0));
}

void SpriteRenderer::InitRenderData()
{
	unsigned int VBO;
	float vertices[] =
	{
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
	};

    GLCall(glGenVertexArrays(1, &this->m_QuadVAO));
    GLCall(glGenBuffers(1, &VBO));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCall(glBindVertexArray(this->m_QuadVAO));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}
