#pragma once

#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "Texture2D.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTAION
#include "stb_image/stb_image.h"


class ResourceManager
{
public:
	static std::map<std::string, Shader>    m_Shaders;
	static std::map<std::string, Texture2D> m_Textures;

	// Load and get Shader
	// TODO: add geometry shader support
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);
	static Shader GetShader(std::string name);

	// Load and get Texture
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
	static Texture2D GetTexture(std::string name);

	static void Clear();

private:
	ResourceManager();
	// TODO: add geometry shader support
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);
};