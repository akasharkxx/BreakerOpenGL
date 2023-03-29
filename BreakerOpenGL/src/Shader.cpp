#include "Shader.h"

Shader::~Shader()
{
}

Shader& Shader::Use()
{
	// TODO: insert return statement here
	GLCall(glUseProgram(this->m_ID));
	return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource)
{
	unsigned sVertex, sFragment;

	// Vertex Shader
	GLCall(sVertex = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(sVertex, 1, &vertexSource, NULL));
	GLCall(glCompileShader(sVertex));
	checkCompileErrors(sVertex, "VERTEX");

	// Fragment Shader
	GLCall(sFragment = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(sFragment, 1, &fragmentSource, NULL));
	GLCall(glCompileShader(sFragment));
	checkCompileErrors(sFragment, "FRAGMENT");

	// TODO: Add Geometry Shader

	// Shader program
	GLCall(this->m_ID = glCreateProgram());
	GLCall(glAttachShader(this->m_ID, sVertex));
	GLCall(glAttachShader(this->m_ID, sFragment));
	// TODO: attach geometry shader
	GLCall(glLinkProgram(this->m_ID));
	checkCompileErrors(this->m_ID, "PROGRAM");

	// Cleanup
	GLCall(glDeleteShader(sVertex));
	GLCall(glDeleteShader(sFragment));
	// TODO: delete geometry shader
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform1f(glGetUniformLocation(this->m_ID, name), value));
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();

	GLCall(glUniform1i(glGetUniformLocation(this->m_ID, name), value));
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform2f(glGetUniformLocation(this->m_ID, name), x, y));
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform2f(glGetUniformLocation(this->m_ID, name), value.x, value.y));
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform3f(glGetUniformLocation(this->m_ID, name), x, y, z));
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform3f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z));
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform4f(glGetUniformLocation(this->m_ID, name), x, y, z, w));
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniform4f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z, value.w));
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), 1, false, glm::value_ptr(matrix)));
}


void Shader::checkCompileErrors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];

	if (type != "PROGRAM")
	{
		GLCall(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(object, 1024, NULL, infoLog));
			LOG("ERROR::SHADER: Compile error: Type: " << type << "\n"
				<< infoLog << "\n-------------------------------------");
		}
	}
	else 
	{
		GLCall(glGetProgramiv(object, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(object, 1024, NULL, infoLog));
			LOG("ERROR::PROGRAM: Link error: Type: " << type << "\n"
				<< infoLog << "\n-------------------------------------");
		}
	}
}
