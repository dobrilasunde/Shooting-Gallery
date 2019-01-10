#include "Shader.hpp"
#include "Texture.hpp"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader(): mShaderProgram(0), mVertexShader(0), mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) || !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform1f(loc, value);
}

void Shader::SetPointLightUniform(const char * name, PointLight lights[], int size)
{
	for (int i = 0; i < size; i++)
	{
		std::string coreName = std::string(name);
		coreName += "[" + std::to_string(i) + "].";

		GLuint loc1 = glGetUniformLocation(mShaderProgram, (coreName + "Position").c_str());
		glUniform3fv(loc1, 1, lights[i].Position.GetAsFloatPtr());

		GLuint loc2 = glGetUniformLocation(mShaderProgram, (coreName + "DiffuseColor").c_str());
		glUniform3fv(loc2, 1, lights[i].DiffuseColor.GetAsFloatPtr());

		GLuint loc3 = glGetUniformLocation(mShaderProgram, (coreName + "SpecularColor").c_str());
		glUniform3fv(loc3, 1, lights[i].SpecularColor.GetAsFloatPtr());

		GLuint loc4 = glGetUniformLocation(mShaderProgram, (coreName + "TurnOn").c_str());
		glUniform1i(loc4, lights[i].TurnOn);
	}
}

void Shader::SetFloatArrayUniform(const char * name, float values[], int size)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform3fv(loc, size, (const GLfloat *)values);
}

void Shader::SetIntUniform(const char * name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{

	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}