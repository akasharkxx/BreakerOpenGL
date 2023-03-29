#pragma once

#include "Renderer.h"

class Texture2D
{
public:
	unsigned int m_ID;

	Texture2D();
	~Texture2D();

	unsigned int m_Width, m_Height;
	
	unsigned int m_InternalFormat;
	unsigned int m_ImageFormat;

	unsigned int m_WrapS;
	unsigned int m_WrapT;
	unsigned int m_FilterMin;
	unsigned int m_FilterMax;

	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	void Bind() const;
};