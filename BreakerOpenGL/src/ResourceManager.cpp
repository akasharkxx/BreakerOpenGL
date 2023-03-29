#include "ResourceManager.h"
std::map<std::string, Texture2D>    ResourceManager::m_Textures;
std::map<std::string, Shader>       ResourceManager::m_Shaders;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name)
{
    m_Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return m_Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return m_Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    m_Textures[name] = loadTextureFromFile(file, alpha);
    return m_Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return m_Textures[name];
}

void ResourceManager::Clear()
{
    for (auto itr : m_Shaders)
    {
        GLCall(glDeleteProgram(itr.second.m_ID));
    }
    for (auto itr : m_Textures)
    {
        GLCall(glDeleteTextures(1, &itr.second.m_ID));
    }
}


Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;

    try
    {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
    
        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // TODO: add geometry shader support
    }
    catch (std::exception e)
    {
        LOG("ERROR::SHADER: Failed to read shader files");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    Texture2D texture;
    if (alpha)
    {
        texture.m_InternalFormat = GL_RGBA;
        texture.m_ImageFormat = GL_RGBA;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}
