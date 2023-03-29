#include "Texture2D.h"

Texture2D::Texture2D()
	: m_Width(0), m_Height(0), m_InternalFormat(GL_RGB),
	m_ImageFormat(GL_RGB), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT),
	m_FilterMin(GL_LINEAR), m_FilterMax(GL_LINEAR)
{
	GLCall(glGenTextures(1, &this->m_ID));
}

Texture2D::~Texture2D()
{
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->m_Width = width;
	this->m_Height = height;
	GLCall(glBindTexture(GL_TEXTURE_2D, this->m_ID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, this->m_InternalFormat, width, height, 0, 
		this->m_ImageFormat, GL_UNSIGNED_BYTE, data));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_WrapS))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_WrapT))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_FilterMin))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_FilterMax))

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, this->m_ID));
}
